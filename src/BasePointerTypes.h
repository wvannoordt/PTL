#ifndef BASE_POINTER_TYPES_H
#define BASE_POINTER_TYPES_H
#include <string>
namespace PropTreeLib
{
    namespace Variables
    {
        enum BasePointer
        {
            None,
            IntPointer,
            DoublePointer,
            BoolPointer,
            IntArrayPointer,
            DoubleArrayPointer,
            BoolArrayPointer,
            StringPointer,
            CustomClassPointers
        };
        static inline std::string BasePointerToString(BasePointer p)
        {
            switch (p)
            {
                case None:                return "BasePointer::None";
                case IntPointer:          return "BasePointer::IntPointer";
                case DoublePointer:       return "BasePointer::DoublePointer";
                case BoolPointer:         return "BasePointer::BoolPointer";
                case IntArrayPointer:     return "BasePointer::IntArrayPointer";
                case DoubleArrayPointer:  return "BasePointer::DoubleArrayPointer";
                case BoolArrayPointer:    return "BasePointer::BoolArrayPointer";
                case StringPointer:       return "BasePointer::StringPointer";
                case CustomClassPointers: return "BasePointer::CustomClassPointers";
                default: return "NO_BASE_PTR";
            }
        }
    }
}

#endif
