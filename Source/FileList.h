#pragma once

#include "JuceHeader.h"

class FileList : public Component,
				 public TableListBoxModel
{
public:
	FileList(const String& file_path);

	void paintRowBackground(Graphics& g, int rowNumber, 
		int /*width*/, int /*height*/, bool rowIsSelected) override;
	void paintCell(Graphics& g, int row_number, int column_id,
		int width, int height, bool row_is_selected) override;
	void resized() override;
	void sortOrderChanged(int new_sort_column_id, bool is_forwards);
	
	Component* refreshComponentForCell(int row_number, int column_id, bool /*isRowSelected*/,
		Component* existing_component_to_update) override;
	
	int getColumnAutoSizeWidth(int columnId) override;
	int getNumRows() override;

	int get_selection(const int row_number) const;
	void set_selection(const int row_number, const int new_selection);
	void set_text(const int column_number, const int row_number, const String& new_text);

	String get_text(const int column_number, const int row_number) const;

private:
	void load_xml_file(const String& file_path);
	const String& get_attribute_name_for_column_id(const int columnId) const;

	TableListBox table{ {}, this };
	Font font{ 14.0f };

	std::unique_ptr<XmlElement> xml_data_;
	XmlElement* column_list_ = nullptr;
	XmlElement* data_list_ = nullptr;
	int num_rows_ = 0;

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

	class DataSorter
	{
	public:
		DataSorter(const String& attribute_to_sort_by, bool forwards)
			: attribute_to_sort_(attribute_to_sort_by),
			direction_(forwards ? 1 : -1)
		{}

		int compareElements(XmlElement* first, XmlElement* second) const
		{
			auto result = first->getStringAttribute(attribute_to_sort_)
				.compareNatural(second->getStringAttribute(attribute_to_sort_));

			if (result == 0)
				result = first->getStringAttribute("ID")
				.compareNatural(second->getStringAttribute("ID"));

			return direction_ * result;
		}

	private:
		const String attribute_to_sort_;
		const int direction_;
	};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileList)
};