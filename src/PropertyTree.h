#ifndef PROPTREE_H
#define PROPTREE_H
#include <iostream>
#include <string>
#include "PropertySection.h"
#include "PropStringHandler.h"
#include "QueryResult.h"
#define TR_PROTOTYPE 123873
#define TR_DATA      232342

namespace PTL
{
    class PropertyTree
    {
        public:
            PropertyTree(void);
            ~PropertyTree(void);
            void ReadInputFileToTreeData(std::string filename);
            void Read(std::string filename);
            void SetCloseMessage(std::string message);
            void DebugPrint(void);
            PropertySection& operator [](std::string argument);
            void StrictParse(void);
            void Serialize(std::ostream& stream);
            void Destroy(void);
            void CreateDefaultValuesFile(std::string filename);
            void SetAsSubtree(PropertySection& newPrincipal);
            void PushSection(std::string pushedSection);
            PropertySection& Principal(void);
            void ResolveAllStrings(void);
            QueryResult Query(std::string sectionQuery);
            std::vector<PropertySection*>::iterator begin() noexcept;
            std::vector<PropertySection*>::const_iterator begin() const noexcept;
            std::vector<PropertySection*>::iterator end() noexcept;
            std::vector<PropertySection*>::const_iterator end() const noexcept;
        private:
            PropertySection* principalSection;
            PropStringHandler stringHandler;
            int treeSpecification;
            std::string closeMessage;
            bool wasCreatedAsSubtree;
    };
}
#endif
