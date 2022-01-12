/** 
 * Copyright 2022 Clement Saad
 * 
 * This file is part of the uLambda Debug Library.
 *
 * The uLambda Debug Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The uLambda Debug Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uLambda Debug Library.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#define ULD_TRACKER_CTOR(T) T(this)

namespace uldl {
	typedef unsigned long usize;

	void init();

	class raw_tracker {
	private:
		void *ptr;
		usize object_size;

	public:
		raw_tracker(void *ptr, usize object_size);
		raw_tracker(const raw_tracker &cpy);
		~raw_tracker();

		raw_tracker &operator=(const raw_tracker &cpy);

		void *get_raw_pointer() const;
		usize get_size() const;
	};

	template<typename _Tp>
	struct tracker: raw_tracker {
		tracker(_Tp *pointer): raw_tracker(reinterpret_cast<void *>(pointer), sizeof(_Tp)) {}

		_Tp *get_pointer() const {
			return reinterpret_cast<_Tp *>(get_raw_pointer());
		}
	};

} // namespace uldl
