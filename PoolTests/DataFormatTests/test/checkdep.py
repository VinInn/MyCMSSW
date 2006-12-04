# for the time being only dataformat
#
import string, os, sys
#
class BuildFile :
    def __init__(self, filen) :
        # print filen
        self.use=[]
        f = open(filen)
        lines = string.join(f.readlines())
        j=0
        while (1):
            i = lines.find("use",j)
            if (i<0) : break
            i = lines.find("name",i)
            if (i<0) : break
            i = lines.find("=",i)
            if (i<0) : break
            j = lines.find('>',i)
            if (j<i) : break
            # print lines[i:j+1]
            self.use.append(lines[i+1:j].strip())
    def depend(self,pack):
        return pack in self.use            
        
class Headers :
    def __init__(self, filen, lpk) :
        self.localpack =lpk
        self.packages=[]
        f = open(filen)
        lines = string.join(f.readlines())
        k=0
        while (1):
            i = lines.find("#include",k)
            if (i<0) : break
            k = lines.find('\n',i)
            if (k<0) : break
            j = lines.rfind('/interface',i,k)
            if (j<i) : continue
            i = lines.rfind('"',i,j)
            if (i<0) : break
            # print lines[i:j+1]
            pk = lines[i+1:j].strip()
            if (pk!=self.localpack) :
                self.packages.append(pk)

 
class CheckDep :
     def __init__(self, src) :
         self.src=src
         self.dirs = ["DataFormats","SimDataFormats"]
         self.report = ''
         
     def check(self) :
         for bdir in self.dirs:
             topdir = self.src+'/'+bdir
             # print topdir
             for dir in os.listdir(topdir):
                 self.report += self.iterate(bdir+'/'+dir)

     def iterate(self,pack) :
         report=''
         buildfile = pack+'/BuildFile'
         try:
             bfile =BuildFile(self.src+'/'+buildfile)
         except:
             report += "Package "+pack+" does not have a BuildFile\n"
         else:
             interface = self.src+'/'+pack+'/interface'
             for hname in os.listdir(interface):
                 # print hname
                 if (hname.find(".h")<1): continue
                 headers = Headers(interface+'/'+hname,pack)
                 for pk in headers.packages :
                     if (not bfile.depend(pk)) :
                         report += 'Missing use of ' + pk \
                                   + ' in ' + pack + ' BuildFile\n'
         return report

def main():
    args = sys.argv[1:]
    if len(args) != 1:
        print 'usage: python checkdep.py $CMSSW_BASE/src'
        sys.exit(-1)
    c = CheckDep(args[0])
    c.check()
    print c.report
    
if __name__ == '__main__':
    main()
