#pragma once
#include <array>
#include <cassert>

template<typename T, uint32_t MAX_SIZE>
class PackedArray {
    using Index = std::uint32_t;
    Index _entry_count;
    std::array<Index, MAX_SIZE> _entry_to_index;
    std::array<Index, MAX_SIZE> _index_to_entry;

public:
    std::array<T, MAX_SIZE> entries;

    PackedArray() : _entry_count(0) {
        for (unsigned int i = 0; i < MAX_SIZE; i++) {
            _entry_to_index[i] = i;
            _index_to_entry[i] = i;
        }
    }

    bool HasData(Index entry) {
        return _entry_to_index[entry] < _entry_count;
    }

    void RemoveData(Index entry) {
        assert(HasData(entry) && "Entry does not have valid data to remove");

        // If not the last one
        if (_entry_to_index[entry] < _entry_count - 1) {
            // Move last one to deleted position
            // Adjust helper arrays
            Index removed_index = _entry_to_index[entry];
            Index last_entry = _index_to_entry[_entry_count - 1];

            _entry_to_index[last_entry] = removed_index;
            _index_to_entry[removed_index] = last_entry;
			
			_entry_to_index[entry] = _entry_count - 1;
			_index_to_entry[_entry_count - 1] = entry;

            // Actually move data
            entries[removed_index] = entries[_entry_count - 1];
        }

        _entry_count--;
    }

    T &GetData(Index entry) {
        assert(HasData(entry) && "Entry does not have valid data");
        
        return entries[_entry_to_index[entry]];
    }

    void SetData(Index entry, const T &data) {
        // If entry is new
        if (_entry_to_index[entry] >= _entry_count) {
            
            _entry_to_index[entry] = _entry_count;
            _index_to_entry[_entry_count] = entry;

            _entry_count++;
        }

        entries[_entry_to_index[entry]] = data;
    }

    // BEWARE RETURED INDEX IS INTERNAL
    // AND THEREFORE SHOULD ONLY BE USED TO ITERATE OVER INTERNAL ARRAY
    Index GetSize() const {
        return _entry_count;
    }
	
	Index size() const {
		return _entry_count;
	}

    Index GetEmptyEntry() const {
        return _index_to_entry[_entry_count];
    }

    Index AddData(const T &data) {
        Index place = GetEmptyEntry();
        SetData(place, data);
        return place;
    }
};
