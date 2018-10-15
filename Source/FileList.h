#pragma once

#include "JuceHeader.h"

//==============================================================================
class FileList : public Component,
	public TableListBoxModel
{
public:
	FileList(const String& file_path)
	{
		load_data(file_path);

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

	int getNumRows() override
	{
		return num_rows_;
	}

	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override
	{
		auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId)
			.interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
		if (rowIsSelected)
			g.fillAll(Colours::lightblue);
		else if (rowNumber % 2)
			g.fillAll(alternateColour);
	}

	void paintCell(Graphics& g, int row_number, int column_id,
		int width, int height, bool row_is_selected) override
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

	void sortOrderChanged(int new_sort_column_id, bool is_forwards) override
	{
		if (new_sort_column_id != 0)
		{
			TutorialDataSorter sorter(get_attribute_name_for_column_id(new_sort_column_id), is_forwards);
			data_list_->sortChildElements(sorter);

			table.updateContent();
		}
	}

	Component* refreshComponentForCell(int row_number, int column_id, bool /*isRowSelected*/,
		Component* existing_component_to_update) override
	{
		if (column_id == 9)
		{
			auto* selection_box = static_cast<SelectionColumnCustomComponent*> (existing_component_to_update);

			if (selection_box == nullptr)
				selection_box = new SelectionColumnCustomComponent(*this);

			selection_box->setRowAndColumn(row_number, column_id);
			return selection_box;
		}

		if (column_id == 8)
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

	int getColumnAutoSizeWidth(int columnId) override
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

	int get_selection(const int row_number) const
	{
		return data_list_->getChildElement(row_number)->getIntAttribute("Select");
	}

	void set_selection(const int row_number, const int new_selection)
	{
		data_list_->getChildElement(row_number)->setAttribute("Select", new_selection);
	}

	String get_text(const int column_number, const int row_number) const
	{
		return data_list_->getChildElement(row_number)->getStringAttribute(get_attribute_name_for_column_id(column_number));
	}

	void set_text(const int column_number, const int row_number, const String& new_text)
	{
		const auto& columnName = table.getHeader().getColumnName(column_number);
		data_list_->getChildElement(row_number)->setAttribute(columnName, new_text);
	}

	//==============================================================================
	void resized() override
	{
		table.setBoundsInset(BorderSize<int>(8));
	}

private:
	TableListBox table{ {}, this };
	Font font{ 14.0f };

	std::unique_ptr<XmlElement> xml_data_;
	XmlElement* column_list_ = nullptr;
	XmlElement* data_list_ = nullptr;
	int num_rows_ = 0;

	//==============================================================================
	class EditableTextCustomComponent : public Label
	{
	public:
		EditableTextCustomComponent(FileList& td)
			: owner_(td)
		{
			setEditable(false, true, false);
		}

		void mouseDown(const MouseEvent& event) override
		{
			owner_.table.selectRowsBasedOnModifierKeys(row, event.mods, false);

			Label::mouseDown(event);
		}

		void textWasEdited() override
		{
			owner_.set_text(column_id_, row, getText());
		}

		void setRowAndColumn(const int newRow, const int newColumn)
		{
			row = newRow;
			column_id_ = newColumn;
			setText(owner_.get_text(column_id_, row), dontSendNotification);
		}

	private:
		FileList& owner_;
		int row, column_id_;
		Colour text_colour_;
	};

	//==============================================================================
	class SelectionColumnCustomComponent : public Component
	{
	public:
		SelectionColumnCustomComponent(FileList& td)
			: owner_(td)
		{
			addAndMakeVisible(toggle_button_);

			toggle_button_.onClick = [this] { owner_.set_selection(row, (int)toggle_button_.getToggleState()); };
		}

		void resized() override
		{
			toggle_button_.setBoundsInset(BorderSize<int>(2));
		}

		void setRowAndColumn(int newRow, int newColumn)
		{
			row = newRow;
			column_id_ = newColumn;
			toggle_button_.setToggleState((bool)owner_.get_selection(row), dontSendNotification);
		}

	private:
		FileList& owner_;
		ToggleButton toggle_button_;
		int row, column_id_;
	};

	//==============================================================================
	class TutorialDataSorter
	{
	public:
		TutorialDataSorter(const String& attribute_to_sort_by, bool forwards)
			: attribute_to_sort_(attribute_to_sort_by),
			direction_(forwards ? 1 : -1)
		{}

		int compare_elements(XmlElement* first, XmlElement* second) const
		{
			auto result = first->getStringAttribute(attribute_to_sort_)
				.compareNatural(second->getStringAttribute(attribute_to_sort_));

			if (result == 0)
				result = first->getStringAttribute("ID")
				.compareNatural(second->getStringAttribute("ID"));

			return direction_ * result;
		}

	private:
		String attribute_to_sort_;
		int direction_;
	};

	//==============================================================================
	void load_data(const String& file_path)
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

	String get_attribute_name_for_column_id(const int columnId) const
	{
		forEachXmlChildElement(*column_list_, columnXml)
		{
			if (columnXml->getIntAttribute("columnId") == columnId)
				return columnXml->getStringAttribute("name");
		}

		return {};
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileList)
};