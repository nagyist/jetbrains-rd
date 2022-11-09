//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.10.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#include "ExampleModelNova.Generated.h"

#include "ExampleModelNova/Selection.Generated.h"
#include "ExampleModelNova/Baz.Generated.h"
#include "ExampleModelNova/FooBar.Generated.h"
#include "ExampleModelNova/Document.Generated.h"
#include "ExampleModelNova/ScalarExample.Generated.h"
#include "ExampleModelNova/TextControl.Generated.h"
#include "ExampleModelNova/EnumSetTest.Generated.h"
#include "ExampleModelNova/Z.Generated.h"
#include "ExampleModelNova/Completion.Generated.h"
#include "ExampleModelNova/Foo_Unknown.Generated.h"
#include "ExampleModelNova/ScalarPrimer_Unknown.Generated.h"
#include "ExampleModelNova/A_Unknown.Generated.h"

#include "ExampleRootNova/ExampleRootNova.Generated.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
#pragma warning( disable:4267 )
#pragma warning( disable:4244 )
#pragma warning( disable:4100 )
#endif

namespace org.example {
// companion

ExampleModelNova::ExampleModelNovaSerializersOwner const ExampleModelNova::serializersOwner;

void ExampleModelNova::ExampleModelNovaSerializersOwner::registerSerializersCore(rd::Serializers const& serializers) const
{
    serializers.registry<Selection>();
    serializers.registry<Baz>();
    serializers.registry<FooBar>();
    serializers.registry<Document>();
    serializers.registry<ScalarExample>();
    serializers.registry<TextControl>();
    serializers.registry<Completion>();
    serializers.registry<Foo_Unknown>();
    serializers.registry<ScalarPrimer_Unknown>();
    serializers.registry<A_Unknown>();
}

void ExampleModelNova::connect(rd::Lifetime lifetime, rd::IProtocol const * protocol)
{
    ExampleRootNova::serializersOwner.registry(protocol->get_serializers());
    
    identify(*(protocol->get_identity()), rd::RdId::Null().mix("ExampleModelNova"));
    bind(lifetime, protocol, "ExampleModelNova");
}

// constants
// initializer
void ExampleModelNova::initialize()
{
    version_.optimize_nested = true;
    serializationHash = -4929039364373743159L;
}
// primary ctor
ExampleModelNova::ExampleModelNova(rd::RdSignal<int32_t, rd::Polymorphic<int32_t>> push_, rd::RdProperty<int32_t, rd::Polymorphic<int32_t>> version_, rd::RdMap<int32_t, Document, rd::Polymorphic<int32_t>, rd::Polymorphic<Document>> documents_, rd::RdMap<ScalarExample, TextControl, rd::Polymorphic<ScalarExample>, rd::Polymorphic<TextControl>> editors_) :
rd::RdExtBase()
,push_(std::move(push_)), version_(std::move(version_)), documents_(std::move(documents_)), editors_(std::move(editors_))
{
    initialize();
}
// secondary constructor
// default ctors and dtors
ExampleModelNova::ExampleModelNova()
{
    initialize();
}
// reader
// writer
// virtual init
void ExampleModelNova::init(rd::Lifetime lifetime) const
{
    rd::RdExtBase::init(lifetime);
    bindPolymorphic(push_, lifetime, this, "push");
    bindPolymorphic(version_, lifetime, this, "version");
    bindPolymorphic(documents_, lifetime, this, "documents");
    bindPolymorphic(editors_, lifetime, this, "editors");
}
// identify
void ExampleModelNova::identify(const rd::Identities &identities, rd::RdId const &id) const
{
    rd::RdBindableBase::identify(identities, id);
    identifyPolymorphic(push_, identities, id.mix(".push"));
    identifyPolymorphic(version_, identities, id.mix(".version"));
    identifyPolymorphic(documents_, identities, id.mix(".documents"));
    identifyPolymorphic(editors_, identities, id.mix(".editors"));
}
// getters
rd::ISignal<int32_t> const & ExampleModelNova::get_push() const
{
    return push_;
}
rd::IProperty<int32_t> const & ExampleModelNova::get_version() const
{
    return version_;
}
rd::IViewableMap<int32_t, Document> const & ExampleModelNova::get_documents() const
{
    return documents_;
}
rd::IViewableMap<ScalarExample, TextControl> const & ExampleModelNova::get_editors() const
{
    return editors_;
}
// intern
// equals trait
// equality operators
bool operator==(const ExampleModelNova &lhs, const ExampleModelNova &rhs) {
    return &lhs == &rhs;
}
bool operator!=(const ExampleModelNova &lhs, const ExampleModelNova &rhs){
    return !(lhs == rhs);
}
// hash code trait
// type name trait
// static type name trait
// polymorphic to string
std::string ExampleModelNova::toString() const
{
    std::string res = "ExampleModelNova\n";
    res += "\tpush = ";
    res += rd::to_string(push_);
    res += '\n';
    res += "\tversion = ";
    res += rd::to_string(version_);
    res += '\n';
    res += "\tdocuments = ";
    res += rd::to_string(documents_);
    res += '\n';
    res += "\teditors = ";
    res += rd::to_string(editors_);
    res += '\n';
    return res;
}
// external to string
std::string to_string(const ExampleModelNova & value)
{
    return value.toString();
}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

