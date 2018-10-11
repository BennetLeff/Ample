/*
  ==============================================================================

    FileList.cpp
    Created: 9 Oct 2018 3:50:05pm
    Author:  bennet

  ==============================================================================
*/

#include "FileList.h"

FileList::FileList()
{
	load_data();

	if (column_list_ != nullptr)
	{
		forEachXmlChildElement(*column_list_, columnXml)
		{
			table.getHeader().addColumn(columnXml->getStringAttribute("name"),
				columnXml->getIntAttribute("columnId"),
				columnXml->getIntAttribute("width"),
				50,
				400,
				TableHeaderComponent::defaultFlags);
		}
	}

	addAndMakeVisible(table);
	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);

	table.getHeader().setSortColumnId(1, true);
	table.getHeader().setColumnVisible(8, false);

	table.setMultipleSelectionEnabled(true);
}

String FileList::get_attribute_name_for_col_id(const int32_t col_id) const
{
	forEachXmlChildElement(*column_list_, columnXml)
	{
		if (columnXml->getIntAttribute("columnId") == col_id)
			return columnXml->getStringAttribute("name");
	}

	return {};
}

void FileList::load_data()
{
	auto dir = File::getCurrentWorkingDirectory();

	uint16_t num_tries = 0;

	while (!dir.getChildFile("Resources").exists() && num_tries < 15)
		dir = dir.getParentDirectory();

	auto table_file = dir.getChildFile("Resources").getChildFile("TableData.xml");

	if (table_file.exists())
	{
		file_listing_.reset(XmlDocument::parse(table_file));

		data_list_ = file_listing_->getChildByName("DATA");
		column_list_ = file_listing_->getChildByName("HEADERS");

		num_rows_ = data_list_->getNumChildElements();
	}
}

void FileList::paintRowBackground(Graphics& g, int row_number, int /*width*/, int /*height*/, bool row_is_selected)
{
	auto alt_colour = getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);

	if (row_is_selected)
		g.fillAll(Colours::lightblue);
	else if (row_number % 2)
		g.fillAll(alt_colour);
}

void FileList::paintCell(Graphics& g, int row_number, int col_id, int width, int height, bool row_is_selected)
{
	g.setColour(row_is_selected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
	g.setFont(font);

	if (auto* row_el = data_list_->getChildElement(row_number))
	{
		auto text = row_el->getStringAttribute(get_attribute_name_for_col_id(col_id));

		g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}

	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

void FileList::sortOrderChanged(int new_sort_col_id, bool is_forwards)
{
	/* do nothing at the moment */
}

Component* FileList::refreshComponentForCell(int row_num, int col_id, bool /*is_row_selected*/,
	Component* existing_comp_to_update)
{
	/* for now just return nullptr */
	return nullptr;
}

int FileList::getColumnAutoSizeWidth(int col_id)
{
	if (col_id == 9)
		return 50;

	int widest = 32;

	for (auto i = getNumRows(); --i >= 0;)
	{
		if (auto* row_el = data_list_->getChildElement(i))
		{
			auto text = row_el->getStringAttribute(get_attribute_name_for_col_id(col_id));

			widest = jmax(widest, font.getStringWidth(text));
		}
	}

	return widest + 8;
}