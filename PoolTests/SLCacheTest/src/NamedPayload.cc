#include "PoolTest/SLCacheTest/interface/NamedPayload.h"
#include "Reflex/Reflex.h"
#
NamedPayload::NamedPayload(const std::string & iname) : m_name(iname) {

}

void NamedPayload::action(Persil::ProtocolDecoder & decoder) {
  seal::reflex::Object meta = decoder.get();
  seal::reflex::Object it = decoder.get();
  NamedPayloadConsumer * c = dynamic_cast<NamedPayloadConsumer *>(decoder.consumer());
  // if (!c) throw ...;
  if (c) (*c)(m_name,meta,it);
  // other possibility is double dispatch (*c)(seal::reflex::Object(this););
  // with meta and it as attribute of this
}
