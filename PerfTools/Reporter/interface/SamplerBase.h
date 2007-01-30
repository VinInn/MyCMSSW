#ifndef PerfTools_SamplerBase_H
#define PerfTools_SamplerBase_H

namespace perftools {

  /*
   * just to manage activation
   */
  class SamplerBase {
  public:
    typedef std::vector<boost::any> Payload;

    SamplerBase(bool aTemplate=false) : m_templ(aTemplate){}

    // will activate in next copy....
    inline activate() { m_templ=false;}

    inline bool isTemplate() const { return m_templ;}

  private:

    bool m_templ; // if true object is just a "template"

  };



#endif //  PerfTools_SampleBase_H
