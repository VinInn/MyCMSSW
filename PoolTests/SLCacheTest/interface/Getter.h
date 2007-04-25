#ifndef Persil_Getter_H
#define Persil_Getter_H


namespace Persil {

  // putter ABI (is it really needed???
  class Putter {
  public:
    virtual ~Putter(){}
    //    virtual void operator()(std::string & stoken, std::string & res)=0;
    virtual const std::string & operator()(std::string & stoken)=0;
    
  };
  

  // Getter ABI
  class Getter {
  public:
    virtual ~Getter(){}
    virtual void operator()(std::istream & str,  const pool::Token& token, 
			    const std::string & typeName)=0;
    
  };
  
  struct GetterFactory {
    virtual  ~GetterFactory(){}
    virtual Getter * create()=0;
  };
  

}
#endif //  Persil_Getter_H
