#pragma once

/*
 * Thanks to David Rowland for this code. From his talk at ADC 2017 on ValueTrees.
 * Distributed freely on Github and in his talk.
 */

#include "JuceHeader.h"

template<typename ObjectType, typename CriticalSectionType = juce::DummyCriticalSection>
class ValueTreeObjectList : public juce::ValueTree::Listener
{
public:
	ValueTreeObjectList(const juce::ValueTree& parent_tree)
		: parent_(parent_tree)
	{
		parent_.addListener(this);
	}

	virtual ~ValueTreeObjectList()
	{
		jassert(objects_.size() == 0); // must call freeObjects() in the subclass destructor!
	}

	// call in the sub-class when being created
	void rebuild_objects()
	{
		jassert(objects_.size() == 0); // must only call this method once at construction

		for (const auto& v : parent_)
			if (is_suitable_type(v))
				if (ObjectType* new_object = create_new_object(v))
					objects_.add(new_object);
	}

	// call in the sub-class when being destroyed
	void freeObjects()
	{
		parent_.removeListener(this);
		deleteAllObjects();
	}

	//==============================================================================
	virtual bool is_suitable_type(const juce::ValueTree&) const = 0;
	virtual ObjectType* create_new_object(const juce::ValueTree&) = 0;
	virtual void delete_object(ObjectType*) = 0;

	virtual void new_object_added(ObjectType*) = 0;
	virtual void object_removed(ObjectType*) = 0;
	virtual void object_order_changed() = 0;

	//==============================================================================
	void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree& tree) override
	{
		if (isChildTree(tree))
		{
			const int index = parent_.indexOf(tree);
			(void)index;
			jassert(index >= 0);

			if (ObjectType* newObject = create_new_object(tree))
			{
				{
					const ScopedLockType sl(arrayLock);

					if (index == parent_.getNumChildren() - 1)
						objects_.add(newObject);
					else
						objects_.addSorted(*this, newObject);
				}

				new_object_added(newObject);
			}
			else
				jassertfalse;
		}
	}

	void valueTreeChildRemoved(juce::ValueTree& exParent, juce::ValueTree& tree, int) override
	{
		if (parent_ == exParent && is_suitable_type(tree))
		{
			const int oldIndex = indexOf(tree);

			if (oldIndex >= 0)
			{
				ObjectType* o;

				{
					const ScopedLockType sl(arrayLock);
					o = objects_.removeAndReturn(oldIndex);
				}

				object_removed(o);
				delete_object(o);
			}
		}
	}

	void valueTreeChildOrderChanged(juce::ValueTree& tree, int, int) override
	{
		if (tree == parent_)
		{
			{
				const ScopedLockType sl(arrayLock);
				sortArray();
			}

			object_order_changed();
		}
	}

	void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override {}
	void valueTreeParentChanged(juce::ValueTree&) override {}

	void valueTreeRedirected(juce::ValueTree&) override { jassertfalse; } // may need to add handling if this is hit

	juce::Array<ObjectType*> objects_;
	CriticalSectionType arrayLock;
	typedef typename CriticalSectionType::ScopedLockType ScopedLockType;

protected:
	juce::ValueTree parent_;

	void deleteAllObjects()
	{
		const ScopedLockType sl(arrayLock);

		while (objects_.size() > 0)
			delete_object(objects_.removeAndReturn(objects_.size() - 1));
	}

	bool isChildTree(juce::ValueTree& v) const
	{
		return is_suitable_type(v) && v.getParent() == parent_;
	}

	int indexOf(const juce::ValueTree& v) const noexcept
	{
		for (int i = 0; i < objects_.size(); ++i)
			if (objects_.getUnchecked(i)->state_ == v)
				return i;

		return -1;
	}

	void sortArray()
	{
		objects_.sort(*this);
	}

public:
	int compareElements(ObjectType* first, ObjectType* second) const
	{
		int index1 = parent_.indexOf(first->state_);
		int index2 = parent_.indexOf(second->state_);
		return index1 - index2;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeObjectList)
};