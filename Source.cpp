#include "CSV reader.h"

int main()
{
	CSVreader object1;
	object1.openFile();
	object1.getData();

	PushToDatabase object2;
	object2.connect();
	object2.pushtoDatabase(object1.fileElements);
}