#ifndef AASL_ERROR_H
#define AASL_ERROR_H
namespace AASL 
{
	class Error 
	{
		public:
		const char* what() const;
	};
}
#endif