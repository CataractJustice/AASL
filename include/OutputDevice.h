#ifndef AASL_OUTPUTDEVICE_H
#define AASL_OUTPUTDEVICE_H
namespace AASL 
{
	class OutputDevice 
	{
		private:
		int id;
		public:
		friend OutputDevice MakeOutputDevice(const char* handle);
	};
}
#endif