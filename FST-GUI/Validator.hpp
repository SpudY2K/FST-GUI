#pragma once
#include "wx/valnum.h"

class WXDLLIMPEXP_CORE wxFlexFloatingPointValidatorBase : public wxNumValidatorBase
{
public:
    void SetFactor(double factor) { m_factor = factor; }

protected:
    typedef double LongestValueType;

    wxFlexFloatingPointValidatorBase(int style)
        : wxNumValidatorBase(style)
    {
        m_factor = 1.0;
    }

    wxString ToString(LongestValueType value) const;
    bool FromString(const wxString& s, LongestValueType* value) const;

    virtual bool IsInRange(LongestValueType value) const = 0;

    virtual bool IsCharOk(const wxString& val, int pos, wxChar ch) const wxOVERRIDE;

private:

    double m_factor;

    wxDECLARE_NO_ASSIGN_CLASS(wxFlexFloatingPointValidatorBase);
};

template <typename T>
class wxFlexFloatingPointValidator
    : public wxPrivate::wxNumValidator<wxFlexFloatingPointValidatorBase, T>
{
public:
    typedef T ValueType;
    typedef wxPrivate::wxNumValidator<wxFlexFloatingPointValidatorBase, T> Base;
    typedef wxFlexFloatingPointValidatorBase::LongestValueType LongestValueType;

    wxFlexFloatingPointValidator(ValueType* value = NULL,
        int style = wxNUM_VAL_DEFAULT)
        : Base(value, style)
    {
        DoSetMinMax();
    }

    virtual wxObject* Clone() const wxOVERRIDE
    {
        return new wxFlexFloatingPointValidator(*this);
    }

    virtual bool IsInRange(LongestValueType value) const wxOVERRIDE
    {
        const ValueType valueT = static_cast<ValueType>(value);

        return this->GetMin() <= valueT && valueT <= this->GetMax();
    }

private:
    void DoSetMinMax()
    {
        // NB: Do not use min(), it's not the smallest representable value for
        //     the floating point types but rather the smallest representable
        //     positive value.
        this->SetMin(-std::numeric_limits<ValueType>::max());
        this->SetMax(std::numeric_limits<ValueType>::max());
    }
};