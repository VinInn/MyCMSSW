import sys, string, os

# write dict fragment for PolyVector
#assume std::vector<std::pair<int,int>> already defined.... 
class  GendirPV :
    def __init__ (self, base, classes) :
        self.base = base
        self.classes = classes
        cltp = '<class name="std::vector<${className} >"/>\n' \
               + '<class name="edm::details::PolyVectorBase<${className}, ${baseName} >"/>\n\n'
        self.classTagTemplate = string.Template(cltp)
        pvtp = '<class name="edm::PolyVector<${baseName} >">\n' \
               + '  <field name="m_types" transient="true"/>\n' \
               + '</class>\n' \
               + '<class name="edm::details::PolyVectorInnerBase<${baseName} >">\n' \
               + '<class name="std::vector<edm::details::PolyVectorInnerBase<${baseName} >*>">\n\n'
        self.PVTagTemplate = string.Template(pvtp)
        self.xml =  self.PVTagTemplate.substitute(baseName=self.base)
        for cl in classes :
            self.xml +=  self.classTagTemplate.substitute(baseName=self.base, className=cl)

    def dictxml(self) :
        return self.xml

def main():
    args = sys.argv[1:]
    if len(args) == 0 :
        print 'usage: python gendirPV.py Base C1 C2 ... CN'
        sys.exit(-1)
    gd = GendirPV(args[0],args[1:])
    print gd.dictxml() 

if __name__ == '__main__':
    main()
