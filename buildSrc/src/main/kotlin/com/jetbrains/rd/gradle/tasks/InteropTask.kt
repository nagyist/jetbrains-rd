package com.jetbrains.rd.gradle.tasks

import com.jetbrains.rd.gradle.tasks.util.portFile
import com.jetbrains.rd.gradle.tasks.util.portFileClosed
import org.gradle.api.DefaultTask
import org.gradle.api.GradleException
import org.gradle.api.Task
import org.gradle.api.tasks.TaskAction
import org.gradle.process.internal.DefaultExecAction
import java.time.LocalTime
import java.util.concurrent.TimeUnit

const val PROCESS_WAIT_TIMEOUT = 20L

/**
 * Executes two native processes provided by taskServer and taskClient,
 * that present server and client respectively.
 */
open class InteropTask : DefaultTask() {
    lateinit var taskServer: MarkedExecTask
    lateinit var taskClient: MarkedExecTask

    private val serverRunningCommand by lazy { taskServer.commandLineWithArgs }
    private val clientRunningCommand by lazy { taskClient.commandLineWithArgs }

    data class NamedProcess(val builder: ProcessBuilder, val name: String)

    private val processes: MutableList<NamedProcess> = mutableListOf()

    private fun outputTmpDirectory() = (taskServer as Task).tmpFileDirectory

    init {
        group = "interop"
    }

    fun lateInit() {
        dependsOn((taskServer as Task).taskDependencies)
        dependsOn((taskClient as Task).taskDependencies)

        System.setProperty("TmpSubDirectory", name)
        outputs.dirs(outputTmpDirectory())
    }

    private fun executeTask(task: MarkedExecTask, command: List<String>) {
        val taskName = task.getName()
        println("Interop task: async running task=$taskName, " +
            "command=${command.joinToString(separator = " ")}, " +
            "working dir=${task.getWorkingDir()}")
        val outputFile = createTempFile(suffix = "$taskName.out")
        println("outputFile=${outputFile}")
        val process = ProcessBuilder(command).apply {
            directory(task.getWorkingDir())
        }
            .redirectErrorStream(true)
            .redirectError(ProcessBuilder.Redirect.INHERIT)
            .redirectOutput(outputFile)

        processes.add(NamedProcess(process, taskName))
    }


    private fun runServer() {
        executeTask(taskServer, serverRunningCommand)
    }

    private fun startClient() {
        executeTask(taskClient, clientRunningCommand)
    }


    private fun beforeStart() {
        assert(portFile.delete())
        assert(portFileClosed.delete())
        System.setProperty("TmpSubDirectory", name)
        assert(outputTmpDirectory().deleteRecursively())
        assert(outputTmpDirectory().mkdirs())
    }

    @TaskAction
    internal fun start() {
        try {
            beforeStart()
            runServer()
            startClient()
        } catch (e: Throwable) {
            throw GradleException("Task $name failed before starting processes", e)
        }

        var taskFailed = false

        processes
            .map { (builder, name) ->
                Pair(builder.start(), name)
            }
            .forEach { (process, name) ->
                try {
                    val exitStatus = process.waitFor(PROCESS_WAIT_TIMEOUT, TimeUnit.SECONDS)
                    if (!exitStatus) {
                        taskFailed = true
                        println("$name exited with status=$exitStatus")
                    }
                    process.destroyForcibly()
                } catch (e: Throwable) {
                    println("Error occurred while process $name executing:")
                    e.printStackTrace()
                    taskFailed = true
                }
            }

        println("At ${LocalTime.now()}: processes're awaited")

        if (taskFailed) {
            throw GradleException("Task $name failed due to executing processes")
        }
    }
}