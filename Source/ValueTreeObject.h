/*
 * Modified version of David Rowland and Harmonaize code.
 */

#pragma once

#include "Identifier.h"

 /*
  *	A run-time class which represents an abstract ValueTree node.
  */
template<const Identifier& identifier_type>
class ValueTreeObject
{
public:
	explicit ValueTreeObject(const ValueTree& v, UndoManager* um)
		: state_(v), undo_manager_(um)
	{
		jassert(v.isValid());
		jassert(v.getType() == identifier);
	}

	virtual ~ValueTreeObject() = default;

	ValueTree& get_state()
	{
		return state_;
	}

	UndoManager* get_undo_manager() const
	{
		return undo_manager_;
	}

	void begin_new_transaction(const String& actionName = String())
	{
		if (undo_manager_)
			undo_manager_->beginNewTransaction(actionName);
	}

	static ValueTree create_default_state()
	{
		return ValueTree(identifier);
	}

	static constexpr const Identifier& identifier = identifier_type;

private:
	ValueTree state_;
	UndoManager* undo_manager_;

	JUCE_DECLARE_NON_COPYABLE(ValueTreeObject)
};