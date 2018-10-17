/*
  ==============================================================================

    FileList.cpp
    Created: 9 Oct 2018 3:50:05pm
    Author:  bennet

  ==============================================================================
*/

#include "FileList.h"

FileList::FileList(const String& folder_path)
{
	const String xml_file_string = create_xml_file(folder_path);
	File xml_file(folder_path + "/TableData2.xml");
		
	xml_file.replaceWithText(xml_file_string);
	load_xml_file(folder_path);

	addAndMakeVisible(table);

	table.setColour(ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness(1);

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

	table.getHeader().setSortColumnId(1, true);
	table.getHeader().setColumnVisible(8, false);

	table.setMultipleSelectionEnabled(true);
}

int FileList::getNumRows()
{
	return num_rows_;
}

void FileList::paintRowBackground(Graphics& g, int rowNumber, int /*width*/,
									int /*height*/, bool rowIsSelected) 
{
	auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
		.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
	if (rowIsSelected)
		g.fillAll(Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll(alternateColour);
}

void FileList::paintCell(Graphics& g, int row_number, int column_id,
							int width, int height, bool row_is_selected)
{
	g.setColour(row_is_selected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId));
	g.setFont(font);

	if (auto* row_element = data_list_->getChildElement(row_number))
	{
		auto text = row_element->getStringAttribute(get_attribute_name_for_column_id(column_id));

		g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}

	g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
	g.fillRect(width - 1, 0, 1, height);
}

void FileList::sortOrderChanged(int new_sort_column_id, bool is_forwards)
{
	if (new_sort_column_id != 0)
	{
		DataSorter sorter(get_attribute_name_for_column_id(new_sort_column_id), is_forwards);
		data_list_->sortChildElements(sorter);

		table.updateContent();
	}
}

Component* FileList::refreshComponentForCell(int row_number, int column_id, bool /*isRowSelected*/,
	Component* existing_component_to_update)
{
	if (column_id == 4)
	{
		auto* selection_box = static_cast<SelectionColumnCustomComponent*> (existing_component_to_update);

		if (selection_box == nullptr)
			selection_box = new SelectionColumnCustomComponent(*this);

		selection_box->setRowAndColumn(row_number, column_id);
		return selection_box;
	}

	if (column_id == 3)
	{
		auto* text_label = static_cast<EditableTextCustomComponent*> (existing_component_to_update);

		if (text_label == nullptr)
			text_label = new EditableTextCustomComponent(*this);

		text_label->setRowAndColumn(row_number, column_id);
		return text_label;
	}

	jassert(existing_component_to_update == nullptr);
	return nullptr;
}

int FileList::getColumnAutoSizeWidth(int columnId)
{
	if (columnId == 9)
		return 50;

	int widest = 32;

	for (auto i = getNumRows(); --i >= 0;)
	{
		if (auto* row_element = data_list_->getChildElement(i))
		{
			auto text = row_element->getStringAttribute(get_attribute_name_for_column_id(columnId));

			widest = jmax(widest, font.getStringWidth(text));
		}
	}

	return widest + 8;
}

int FileList::get_selection(const int row_number) const
{
	return data_list_->getChildElement(row_number)->getIntAttribute("Select");
}

void FileList::set_selection(const int row_number, const int new_selection)
{
	data_list_->getChildElement(row_number)->setAttribute("Select", new_selection);
}

String FileList::get_text(const int column_number, const int row_number) const
{
	return data_list_->getChildElement(row_number)->getStringAttribute(get_attribute_name_for_column_id(column_number));
}

void FileList::set_text(const int column_number, const int row_number, const String& new_text)
{
	const auto& columnName = table.getHeader().getColumnName(column_number);
	data_list_->getChildElement(row_number)->setAttribute(columnName, new_text);
}

void FileList::resized()
{
	table.setBoundsInset(BorderSize<int>(8));
}

void FileList::load_xml_file(const String& file_path)
{
	auto dir = File(file_path);

	int numTries = 0;

	while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
		dir = dir.getParentDirectory();

	auto tableFile = dir.getChildFile("Resources").getChildFile("TableData.xml");

	if (tableFile.exists())
	{
		xml_data_.reset(XmlDocument::parse(tableFile));

		data_list_ = xml_data_->getChildByName("DATA");
		column_list_ = xml_data_->getChildByName("HEADERS");

		num_rows_ = data_list_->getNumChildElements();
	}
}

const String FileList::create_xml_file(const String& folder_path)
{
	XmlElement table_data("TABLE_DATA");
	XmlElement* headers = new XmlElement("HEADERS");
	
	auto header_col_0 = new XmlElement("COLUMN");
	header_col_0->setAttribute("columnId", "1");
	header_col_0->setAttribute("name", "ID");
	header_col_0->setAttribute("width", "50");

	auto header_col_1 = new XmlElement("COLUMN");
	header_col_1->setAttribute("columnId", "2");
	header_col_1->setAttribute("name", "Name");
	header_col_1->setAttribute("width", "200");
	
	auto header_col_2 = new XmlElement("COLUMN");
	header_col_2->setAttribute("columnId", "3");
	header_col_2->setAttribute("name", "Description");
	header_col_2->setAttribute("width", "300");
	
	auto header_col_3 = new XmlElement("COLUMN");
	header_col_3->setAttribute("columnId", "4");
	header_col_3->setAttribute("name", "ID");
	header_col_3->setAttribute("width", "50");

	headers->addChildElement(header_col_0);
	headers->addChildElement(header_col_1);
	headers->addChildElement(header_col_2);
	headers->addChildElement(header_col_3);

	XmlElement* data = new XmlElement("DATA");

	DirectoryIterator iter(File(folder_path), true, "*.wav");
	int count = 1;
	while (iter.next())
	{
		XmlElement* row = new XmlElement("Sample");

		row->setAttribute("ID", "0");
		row->setAttribute("Name", iter.getFile().getFileName());
		row->setAttribute("Description", "...");
		row->setAttribute("Select", String(count));
		
		count++;

		data->addChildElement(row);
	}

	table_data.addChildElement(headers);
	table_data.addChildElement(data);

	auto doc = table_data.createDocument(String());

	return doc;
}

const String& FileList::get_attribute_name_for_column_id(const int columnId) const
{
	forEachXmlChildElement(*column_list_, columnXml)
	{
		if (columnXml->getIntAttribute("columnId") == columnId)
			return columnXml->getStringAttribute("name");
	}

	return {};
}