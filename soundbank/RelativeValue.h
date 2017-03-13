/*
  ==============================================================================

    RelativeValue.h
    Created: 6 Jun 2016 5:11:28pm
    Author:  ZhouFeng

  ==============================================================================
*/

#ifndef RELATIVEVALUE_H_INCLUDED
#define RELATIVEVALUE_H_INCLUDED

template <class TYPE>

class CRelativeValue
{
public:
	CRelativeValue()
	{
		minVal = 0;
		maxVal = 127;
		currentVal = snapVal = 0;
	}
	~CRelativeValue()
	{
	}

	const CRelativeValue& operator= (const CRelativeValue& other)
	{
		snapVal = other.snapVal;
		minVal = other.minVal;
		maxVal = other.maxVal;
		currentVal = other.currentVal;
		return *this;
	}

	//return current value
	TYPE getValue() const
	{
		return currentVal;
	}

    //return the difference between the snap value and the new value.
	TYPE getDifValue(TYPE newValue) const
	{
		return (newValue - snapVal);
	}
    
    //set the new value directly. And the new value will save to snap value.
	void setValue(TYPE newVal, bool updateSnap = true)
	{
		if (newVal < minVal)
		{
			newVal = minVal;
		}
		else if (newVal > maxVal)
		{
			newVal = maxVal;
		}
		currentVal = newVal;
		if (updateSnap)
		{
			snapVal = newVal;
		}
	}
    
    //set the new value with relative value.
	void setRelativeValue(TYPE difValue)
	{
		currentVal = snapVal + difValue;

		if (currentVal < minVal)
		{
			currentVal = minVal;
		}
		else if (currentVal > maxVal)
		{
			currentVal = maxVal;
		}
	}
    
    //set the min/max value.
	void setRange(TYPE min, TYPE max)
	{
		minVal = min;
		maxVal = max;
	}
    
    //
	void saveSnap()
	{
		snapVal = currentVal;
	}

	//
	TYPE updateValue(const CRelativeValue &other, TYPE newValue, bool isAbsolute)
	{
		if (isAbsolute)
		{
			setValue(newValue);
		}
		else
		{
			setRelativeValue(other.getDifValue(newValue));
		}
		return getValue();
	}
protected:
	TYPE snapVal;
	TYPE minVal;
	TYPE maxVal;
	TYPE currentVal;
};

#endif  // RELATIVEVALUE_H_INCLUDED
