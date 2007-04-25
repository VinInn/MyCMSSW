#include "PoolTest/SLCacheTest/interface/NamedPayload.h"
#include "Reflex/Reflex.h"
#
NamedPayload::NamedPayload(const std::string & iname) : m_name(iname) {

}

void NamedPayload::action(Persil::ProtocolDecoder & decoder) {
  ROOT::Reflex::Object meta = decoder.get();
  ROOT::Reflex::Object it = decoder.get();
  NamedPayloadConsumer * c = dynamic_cast<NamedPayloadConsumer *>(decoder.consumer());
  // if (!c) throw ...;
  if (c) (*c)(m_name,meta,it);
  // other possibility is double dispatch (*c)(ROOT::Reflex::Object(this););
  // with meta and it as attribute of this
}
