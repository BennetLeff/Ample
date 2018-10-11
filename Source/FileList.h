/*
  ==============================================================================

    FileList.h
    Created: 9 Oct 2018 3:50:05pm
    Author:  bennet

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FileList : public Component,
				 public TableListBoxModel
{
public:
	FileList();
	int getColumnAutoSizeWidth(int col_id) override;
	int getNumRows() override { return num_rows_; }
	void paintRowBackground(Graphics& g, int row_number, int /*width*/, int /*height*/, bool row_is_selected) override;
	void paintCell(Graphics& g, int row_number, int col_id, int width, int height, bool row_is_selected) override;
	void sortOrderChanged(int new_sort_col_id, bool is_forwards) override;
	Component* refreshComponentForCell(int row_num, int col_id, bool /*is_row_selected*/,
		Component* existing_comp_to_update) override;
	
private:
	String get_attribute_name_for_col_id(const int32_t col_id) const;
	void load_data();

	std::unique_ptr<XmlElement> file_listing_;
	XmlElement* column_list_ = nullptr;
	XmlElement* data_list_ = nullptr;

	TableListBox table{ {}, this };
	Font font{ 14.0f };
	uint16_t num_rows_ = 0;
};