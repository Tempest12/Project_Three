#ifndef COLOUR4F_HPP
#define COLOUR4F_HPP

class Colour4f
{
	//Properties
	public:
		float red;
		float green;
		float blue;
		float alpha;
	protected:
	private:

	//Functions
	public:
		Colour4f(void)
		{
			this -> red = 1.0f;
			this -> green = 1.0f;
			this -> blue = 1.0f;
			this -> alpha = 1.0f;
		}

		Colour4f(float all)
		{
			this -> red = all;
			this -> green = all;
			this -> blue = all;
			this -> alpha = 1.0f;
		}

		Colour4f(float red, float green, float blue, float alpha)
		{
			this -> red = red;
			this -> green = green;
			this -> blue = blue;
			this -> alpha = alpha;
		}

		Colour4f(Colour4f* that)
		{
			this -> red = that -> red;
			this -> green = that -> green;
			this -> blue = that -> blue;
			this -> alpha = that -> alpha;
		}

		~Colour4f(void)
		{
			return;
		}
	protected:
	private:
};



#endif