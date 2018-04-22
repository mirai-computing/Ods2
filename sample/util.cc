#include "util.hh"

#include <ods/ods>

#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace util { // util::

QString
FindFile(const QString &file_name)
{
	QStringList list = QStandardPaths::standardLocations
		(QStandardPaths::DocumentsLocation);
	
	if (list.isEmpty())
		return QLatin1String();
	
	QString dir_path = list[0];
	QString full_path = dir_path + QLatin1String("/ods_test_files/") + file_name;
	
	if (QFile(full_path).exists())
		return full_path;
	
	auto ba = full_path.toLocal8Bit();
	mtl_warn("File not found: %s", ba.data());
	return QLatin1String();
}

void
PrintBorder(ods::Cell *cell, const int row, const int col_index)
{
	mtl_line("Cell at %d:%d...", row, col_index);
	
	if (cell == nullptr)
	{
		mtl_warn("Cell == nullptr");
		return;
	}
	
	ods::inst::StyleStyle *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("style == nullptr");
		return;
	}
	
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		style->Get(ods::id::StyleTableCellProperties);
	
	if (tcp == nullptr)
	{
		mtl_warn("no table cell properties");
		return;
	}
	
	ods::attr::Border *next = tcp->border();
	
	if (next == nullptr)
	{
		mtl_line("border is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_line("border: %s", ba.data());
	}
	
	next = tcp->border_left();
	
	if (next == nullptr)
	{
		mtl_line("border left is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_line("border left: %s", ba.data());
	}
	
	next = tcp->border_top();
	
	if (next == nullptr)
	{
		mtl_line("border top is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_line("border top: %s", ba.data());
	}
	
	next = tcp->border_right();
	
	if (next == nullptr)
	{
		mtl_line("border right is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_line("border right: %s", ba.data());
	}
	
	next = tcp->border_bottom();
	
	if (next == nullptr)
	{
		mtl_line("border bottom is null");
	} else {
		auto ba = next->toString().toLocal8Bit();
		mtl_line("border bottom: %s", ba.data());
	}
}

void
PrintPercentage(ods::Cell *cell)
{
	mtl_line("Cell column index: %d", cell->QueryStart());
	
	if (!cell->is_percentage())
	{
		mtl_warn("Cell != percentage");
		return;
	}
	
	double value = *cell->as_percentage();
	mtl_line("Cell percentage is: %f", value);
	
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("Cell has no style");
		return;
	}
	
	auto *ps = style->GetPercentageStyle();
	
	if (ps == nullptr)
	{
		mtl_warn("Cell has no percentage style");
		return;
	}
	
	auto *ns = ps->GetNumberStyle();
	
	if (ns == nullptr)
	{
		mtl_warn("Percentage style has no number style");
		return;
	}
	
	mtl_line("Min integer digits: %d, min decimal places: %d",
		ns->min_integer_digits(), ns->min_decimal_places());
	
}

void
PrintWidth(ods::inst::TableTableColumn *col)
{
	if (col == nullptr)
	{
		mtl_warn("col == null");
		return;
	}
	
	ods::inst::StyleStyle *style = col->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("No style");
		return;
	}
	
	auto *tcp = (ods::inst::StyleTableColumnProperties*)
		style->Get(ods::id::StyleTableColumnProperties);
	
	if (tcp == nullptr)
	{
		mtl_warn("No table column properties");
		return;
	}
	
	auto *width = tcp->column_width();
	
	if (width == nullptr)
	{
		mtl_warn("No width set");
		return;
	}
	
	auto ba = width->toString().toLocal8Bit();
	mtl_line("Column width: %s", ba.data());
}

void
Save(ods::Book *book)
{
	QFile as(QDir::home().filePath("out.ods"));
	book->Save(as);
}

void
SetHAlignment(ods::inst::StyleStyle *style, const ods::halign::Value a)
{
	auto *spp = (ods::inst::StyleParagraphProperties*)
		style->Get(ods::id::StyleParagraphProperties);
	
	if (spp == nullptr)
		spp = style->NewParagraphProperties();
	
	ods::HAlign ha(a);
	spp->text_align(&ha);
}

void
SetPercentage(ods::inst::StyleStyle *style, const int min_integer_digits,
	const int min_decimal_places)
{
	auto *percent_style = style->GetPercentageStyle();
	
	if (percent_style == nullptr)
		percent_style = style->NewPercentageStyle();
	
	auto *nn = percent_style->GetNumberStyle();
	
	if (nn == nullptr)
		nn = percent_style->NewNumberStyle();
	
	{
		nn->decimal_places(0); // needed for some reason
		nn->min_integer_digits(min_integer_digits);
		nn->min_decimal_places(min_decimal_places);
		// For example:
		// min_decimal_places=4 && min_integer_digits=3 makes 0.83 be
		// displayed as "083.0000%"
		// Note: Calligra Sheets doesn't display percentage formatting properly
	}
	
	auto *nt = (ods::inst::NumberText*)percent_style->Get(ods::id::NumberText);
	
	if (nt == nullptr)
		nt = percent_style->NewNumberText();
	
	nt->SetFirstString(QLatin1String("%"));
}

void
SetVAlignment(ods::inst::StyleStyle *style, const ods::valign::Value a)
{
	auto *tcp = (ods::inst::StyleTableCellProperties*)
		style->Get(ods::id::StyleTableCellProperties);
	
	if (tcp == nullptr)
		tcp = style->NewTableCellProperties();
	
	ods::VAlign va(a);
	tcp->vertical_align(&va);
}

} // util::
