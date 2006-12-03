import sys, os, string


# loops over DataFormats
# find classes.h
# extract edm::wrapper<.....>
# generates a header that includes
#    the classes.h in question
#    typedef edm::wrapper<.....> TheWrapper;
#
class Generator :
     def __init__(self, src) :
         self.src=src
         self.dirs = ["DataFormats","SimDataFormats"]
         self.match="edm::Wrapper"
         self.trans=string.maketrans('<>:','___')
         self.wrappers =[]

     def generate(self) :
         for bdir in self.dirs:
             topdir = self.src+'/'+bdir
             # print topdir
             for dir in os.listdir(topdir):
                 self.iterate(bdir+'/'+dir)

     def iterate(self,sdir) :
         classDotH = sdir+'/src/classes.h'
         try:
             f = open(self.src+'/'+classDotH)
             try:
                 lines = string.join(f.readlines())
                 j=0
                 while (1):
                     i = lines.find(self.match,j)
                     if (i<0) : break
                     j = lines.find(';',i)
                     if (j<0) : break
                     j = lines.rfind('>',i,j)
                     if (j<i) : break
                     # print lines[i:j+1]
                     self.wrappers.append((lines[i:j+1],classDotH))
             finally:
                 f.close();
         except:
             print "no ",classDotH

     def dump(self) :
         print self.wrappers

     def genHeaders(self) :
         for (c,h) in self.wrappers :
             name = c.translate(self.trans).replace(' ','')
             hname = name +'genH.h'
             fname = name +'evts.root'
             f=open(hname,'w')
             f.write('#include "'+h+'"\n')
             f.write('#include <string>\n')
             f.write('typedef '+c+' TheWrapper;\n')
             f.write('namespace {\n')
             f.write('   std::string const filename('+fname+');\n}\n\n') 
             f.close()
def main():
    args = sys.argv[1:]
    if len(args) != 1:
        print 'usage: python generateTests.py $CMSSW_BASE/src'
        sys.exit(-1)
    generator = Generator(args[0])
    generator.generate()
    generator.genHeaders()

if __name__ == '__main__':
    main()
