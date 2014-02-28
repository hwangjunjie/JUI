#include "JuiReader.h"
#include "container/JuiContainer.h"
#include "controls/JuiListView.h"
#include "controls/JuiRadioButton.h"
#include "String/JStringFunction.h"
#include "FileSystem/JFileSystem.h"

#include "JGlobalDefine.h"


void ParseName(JuiControl *com, char* line)
{
	char *find = strchr(line, '(');
	if(find == NULL)
		return;

	*find = 0;
	line = find+1;

	find = strrchr(line, ')');
	if(find == NULL)
		return;

	*find = 0;

	if(com != NULL)
		com->SetName(JStringFunction::Trim(line));
}

bool JuiReader::LoadScript( JuiContainer *root, const char* filename )
{
	char buffer[1024];
	char *line, *find, *name, *value;
	JuiControl *objStack[32] = {0};
	JuiControl *currObject = NULL;
	int stackIndex = -1;

	JFileSystem::GetInstance().EnterPath(filename);

	FILE *pf = fopen(filename, "rb");

	if(pf == NULL)
		return false;

	while(fgets(buffer, sizeof(buffer), pf))
	{
		line = JStringFunction::Trim(buffer);
		if(line[0] == ';' || line[0] == '#' || line[0] == '\0')
			continue;

		if(strnicmp(line, "new", 3) == 0)
		{
			if(currObject != NULL)
			{
				objStack[++stackIndex]  = currObject;
				currObject = NULL;
			}

			line += 4;
			size_t len = strcspn(line, " (");
			char tmp = line[len];
			line[len] = 0;
			line = JStringFunction::Trim(line);
			currObject = dynamic_cast<JuiControl*>(JCreateDynamicObject(line));
			line += len;
			*line = tmp;

			if(currObject != NULL)
				ParseName(currObject, line);
		}
		else if(strnicmp(line, "end", 3) == 0)
		{
			if(stackIndex < 0)
				break;

			JuiContainer *parent = dynamic_cast<JuiContainer*>(objStack[stackIndex]);
			if(parent != NULL)
				parent->AddControl(currObject);

			currObject = parent;
			stackIndex--;
		}
		else if(strnicmp(line, "root", 4) == 0)
		{
			currObject = root;
			ParseName(currObject, line);
		}
		else if(find = strchr(line, '='))
		{
			*find = 0;
			find++;

			name = JStringFunction::TrimRight(line);
			value = JStringFunction::TrimLeft(find);

			if(currObject != NULL)
 				currObject->SetProperty(name, value);
		}
	}

	fclose(pf);
	return true;
}