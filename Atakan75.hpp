#include <cmath>
#include <random>

namespace Atakan75Protect
{
	__forceinline int one()
	{
		return (int)__TIME__;
	}
	__forceinline char* two()
	{
		return (char*)__TIME__;
	}
	__forceinline float three()
	{
		return (int)__TIME__;
	}
	__forceinline int four()
	{
		return pow((int)__TIME__, (unsigned int)__DATE__);
	}
	__forceinline float five()
	{
		return sqrt((int)__TIME__);
	}
	__forceinline int six()
	{
		return (int)__TIME__ + (int)__DATE__;
	}
	__forceinline char* seven()
	{
		return __DATE__;
	}
	__forceinline float eight()
	{
		return (int)__TIME__ / (int)__DATE__;
	}
	__forceinline int nine()
	{
		return pow((int)__TIME__, (unsigned int)__DATE__) / (int)__TIME__;
	}
	__forceinline float ten()
	{
		return sqrt((int)__TIME__ + (int)__DATE__);
	}
	__forceinline int junk1()
	{
		switch (((int)__TIME__ + (int)__DATE__ + rand()) % 10)
		{
		case 0:
			return eight();
		case 1:
			return nine();
		case 2:
			return (int)two();
		case 3:
			return five();
		case 4:
			return (int)seven();
		case 5:
			return four();
		case 6:
			return ten();
		case 7:
			return three();
		case 8:
			return one();
		case 9:
			return six();
		}
	}

	__forceinline char junk2()
	{
		switch ((junk1() + rand()) % 10)
		{
		case 0:
			return five();
		case 1:
			return nine();
		case 2:
			return (int)two();
		case 3:
			return eight();
		case 4:
			return (int)seven();
		case 5:
			return four();
		case 6:
			return ten();
		case 7:
			return three();
		case 8:
			return one();
		case 9:
			return six();
		}
	}

	__forceinline float junk3()
	{
		switch ((junk2() + junk1() + rand()) % 10)
		{
		case 0:
			return (int)two();
		case 1:
			return nine();
		case 2:
			return eight();
		case 3:
			return five();
		case 4:
			return (int)seven();
		case 5:
			return three();
		case 6:
			return ten();
		case 7:
			return four();
		case 8:
			return one();
		case 9:
			return six();
		}
	}

	__forceinline int junk75()
	{
		switch ((junk2() + junk1() + rand()) % 10)
		{
		case 0:
			return (int)two();
		case 1:
			return three();
		case 2:
			return (int)seven();
		case 3:
			return nine();
		case 4:
			return four();
		case 5:
			return five();
		case 6:
			return ten();
		case 7:
			return one();
		case 8:
			return eight();
		case 9:
			return six();
		}
	}

	__forceinline int ju75nk75()
	{
		switch ((junk2() + junk75() + rand()) % 10)
		{
		case 0:
			return (int)two();
		case 1:
			return three();
		case 2:
			return (int)seven();
		case 3:
			return five();
		case 4:
			return ten();
		case 5:
			return five();
		case 6:
			return one();
		case 7:
			return one();
		case 8:
			return eight();
		case 9:
			return five();
		}
	}

	__forceinline int koruma()
	{
		int junk;
		switch (((int)__DATE__ + rand()) % 10)
		{
		case 0:
			junk = junk1();
		case 1:
			junk += junk3();
		case 2:
			junk += junk2();
		case 3:
			junk = junk3();
		case 4:
			junk += junk1();
		case 5:
			junk -= junk2();
		case 6:
			junk -= junk3();
		case 7:
			junk = junk3();
		case 8:
			junk = junk2();
		case 9:
			junk = junk75();
		case 10:
			return junk;
		}
	}

	__forceinline int koruma75()
	{
		int junk;
		switch (((int)__TIME__ + rand()) % 10)
		{
		case 0:
			junk = junk3();
		case 1:
			junk += junk3();
		case 2:
			junk += junk1();
		case 3:
			junk = junk75();
		case 4:
			junk += junk2();
		case 5:
			junk -= junk3();
		case 6:
			junk = junk3();
		case 7:
			junk += junk2();
		case 8:
			junk = junk2();
		case 9:
			junk = junk1();
		case 10:
			return junk;
		}
	}

	__forceinline int seventyfiveprotect()
	{
		int junk;
		switch (((int)__TIME__ + rand()) % 10)
		{
		case 0:
			junk = junk3();
		case 1:
			junk += junk3();
		case 2:
			junk += junk1();
		case 3:
			junk = junk75();
		case 4:
			junk += junk2();
		case 5:
			junk -= junk3();
		case 6:
			junk = junk3();
		case 7:
			junk += junk2();
		case 8:
			junk = junk2();
		case 9:
			junk = junk1();
		case 10:
			return junk;
		}
	}
	__forceinline int seventy75fiveprotect()
	{
		int junk;
		switch (((int)__DATE__ + rand()) % 10)
		{
		case 0:
			junk = junk75();
		case 1:
			junk += junk3();
		case 2:
			junk += ju75nk75();
		case 3:
			junk = junk75();
		case 4:
			junk += junk2();
		case 5:
			junk -= junk3();
		case 6:
			junk = ju75nk75();
		case 7:
			junk += junk75();
		case 8:
			junk = junk2();
		case 9:
			junk = ju75nk75();
		case 10:
			return junk;
		}
	}

	__forceinline int prtct()
	{
		int seventyfive;
		switch (((int)__TIME__ + rand()) % 10)
		{
		case 0:
			seventyfive = koruma();
		case 1:
			seventyfive += koruma75();
		case 2:
			seventyfive -= seventy75fiveprotect();
		case 3:
			return seventyfive;
		}
	}

}