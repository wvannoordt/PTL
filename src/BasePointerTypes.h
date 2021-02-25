#ifndef BASE_POINTER_TYPES_H
#define BASE_POINTER_TYPES_H
#include <string>
namespace PTL
{
    enum BasePointer
    {
        None,
        IntPointer,
        DoublePointer,
        BooleanPointer,
        IntArrayPointer,
        DoubleArrayPointer,
        BooleanArrayPointer,
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
            case BooleanPointer:      return "BasePointer::BooleanPointer";
            case IntArrayPointer:     return "BasePointer::IntArrayPointer";
            case DoubleArrayPointer:  return "BasePointer::DoubleArrayPointer";
            case BooleanArrayPointer: return "BasePointer::BooleanArrayPointer";
            case StringPointer:       return "BasePointer::StringPointer";
            case CustomClassPointers: return "BasePointer::CustomClassPointers";
            default: return "NO_BASE_PTR";
        }
    }
}

#endif
