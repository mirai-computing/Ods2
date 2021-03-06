#include "NumberTextStyle.hpp"

#include "NumberTextContent.hpp"

#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

NumberTextStyle::NumberTextStyle(Abstract *parent, Tag *tag) :
Abstract(parent, parent->ns(), id::NumberTextStyle)
{
	if (tag != nullptr)
		Init(tag);
}

NumberTextStyle::NumberTextStyle(const NumberTextStyle &cloner)
: Abstract(cloner)
{}

NumberTextStyle::~NumberTextStyle()
{}

Abstract*
NumberTextStyle::Clone(Abstract *parent) const
{
	auto *p = new NumberTextStyle(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	p->style_name_ = style_name_;

	return p;
}

void
NumberTextStyle::Init(ods::Tag *tag)
{
	tag->Copy(ns_->style(), ods::ns::kName, style_name_);
	Scan(tag);
}

void
NumberTextStyle::Scan(ods::Tag *tag)
{
	foreach (auto *x, tag->nodes())
	{
		if (AddText(x))
			continue;
		
		auto *next = x->as_tag();
		
		if (next->Is(ns_->number(), ods::ns::kTextContent))
		{
			Append(new NumberTextContent(this, next));
		} else {
			Scan(next);
		}
	}
}

void
NumberTextStyle::WriteData(QXmlStreamWriter &xml)
{
	Write(xml, ns_->style(), ods::ns::kName, style_name_);
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
