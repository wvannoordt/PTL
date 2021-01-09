#ifndef PTL_GENERIC_OBJ_H
#define PTL_GENERIC_OBJ_H
#include "PropertyTree.h"
#include "Error.h"
namespace PTL
{
    class PTLGenericObject
    {
        public:
            virtual void Definitions(PropertySection& section)
            {
                ErrorKill("Object construction attempted with no definition!");
            }
            void Parse(void)
            {
                Definitions(*section);
                objName = section->GetSectionName();
                section->StrictParse();
            }
            PTLGenericObject(PropertySection* const& section_in)
            {
                section = section_in;
            }
        protected:
            PropertySection* section;
            std::string objName;
    };
}

#endif