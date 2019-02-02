/*
  ==============================================================================

    FileList.cpp
    Created: 9 Oct 2018 3:50:05pm
    Author:  bennet

  ==============================================================================
*/

#include "FileList.h"

FileList::FileList(const String& folder_path, std::shared_ptr<Sequencer> sequencer)
{
	const String xml_file_string = create_xml_file(folder_path);
	File xml_file(folder_path + "/TableData2.xml");
		
	xml_file.replaceWithText(xml_file_string);
	load_xml_file(folder_path);

	addAndMakeVisible(table_);

	table_.setColour(ListBox::outlineColourId, Colours::grey);
	table_.setOutlineThickness(1);

	if (column_list_ != nullptr)
	{
		forEachXmlChildElement(*column_list_, columnXml)
		{
			table_.getHeader().addColumn(columnXml->getStringAttribute("name"),
				columnXml->getIntAttribute("columnId"),
				columnXml->getIntAttribute("width"),
				50,
				400,
				TableHeaderComponent::defaultFlags);
		}
	}

	table_.getHeader().setSortColumnId(1, true);
	table_.getHeader().setColumnVisible(8, false);

	table_.setMultipleSelectionEnabled(true);

	sequencer_ = sequencer;

	// cello_sample = std::make_shared<SampleSource>();
	// cello_sample->set_file_path("C:\\Users\\bennet\\Documents\\Workspace\\Ample\\Resources\\cello.wav");
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
	g.setFont(font_);

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

		table_.updateContent();
	}
}

Component* FileList::refreshComponentForCell(int row_number, int column_id, bool /*isRowSelected*/,
	Component* existing_component_to_update)
{
    switch (column_id)
	{
	    case 3:
        {
            auto* text_label = static_cast<EditableTextBox*> (existing_component_to_update);

            if (text_label == nullptr)
                text_label = new EditableTextBox(*this);

            text_label->setRowAndColumn(row_number, column_id);
            return text_label;
        }
	    case 4:
        {
            auto* selection_box = static_cast<SelectionBox*> (existing_component_to_update);

            if (selection_box == nullptr)
                selection_box = new SelectionBox(*this);

            selection_box->setRowAndColumn(row_number, column_id);
            return selection_box;
        }
        case 5:
        {
            auto* track_assignment_entry = static_cast<EditableTextBox*> (existing_component_to_update);

            if (track_assignment_entry == nullptr)
                track_assignment_entry = new EditableTextBox(*this);

            track_assignment_entry->setRowAndColumn(row_number, column_id);
            return track_assignment_entry;
        }
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

			widest = jmax(widest, font_.getStringWidth(text));
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
	const auto& columnName = table_.getHeader().getColumnName(column_number);
	data_list_->getChildElement(row_number)->setAttribute(columnName, new_text);
}

void FileList::resized()
{
	table_.setBoundsInset(BorderSize<int>(8));
}

void FileList::load_xml_file(const String& file_path)
{
	auto dir = File(file_path);

	int numTries = 0;

	while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
		dir = dir.getParentDirectory();

	auto tableFile = dir.getChildFile("Resources").getChildFile("TableData2.xml");

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
	header_col_3->setAttribute("name", "Selected");
	header_col_3->setAttribute("width", "50");

	auto header_col_4 = new XmlElement("COLUMN");
	header_col_4->setAttribute("columnId", "5");
	header_col_4->setAttribute("name", "TrackAssignment");
	header_col_4->setAttribute("width", "300");

	headers->addChildElement(header_col_0);
	headers->addChildElement(header_col_1);
	headers->addChildElement(header_col_2);
	headers->addChildElement(header_col_3);
	headers->addChildElement(header_col_4);

	XmlElement* data = new XmlElement("DATA");

	DirectoryIterator iter(File(folder_path), true, "*.wav");
	int count = 1;
	while (iter.next())
	{
		XmlElement* row = new XmlElement("Sample");

		row->setAttribute("ID", count);
		row->setAttribute("Name", iter.getFile().getFileName());
		row->setAttribute("Description", "...");
		row->setAttribute("Select", String(count));
		row->setAttribute("TrackAssignment", "");
		
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

	return {}
	;
}