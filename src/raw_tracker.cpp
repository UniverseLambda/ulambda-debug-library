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

#include <uldl_rt.hpp>

namespace uldl {
	raw_tracker::raw_tracker(void *ptr, usize object_size): ptr(ptr), object_size(object_size) {
		__impl::register_pointer(ptr, object_size);
	}
	
	raw_tracker::raw_tracker(const raw_tracker &cpy): ptr(cpy.ptr), object_size(cpy.object_size) {
		__impl::register_pointer(ptr, object_size);
	}
	
	raw_tracker::~raw_tracker() {
		__impl::remove_pointer(ptr);
	}
	
	raw_tracker &raw_tracker::operator=(const raw_tracker &cpy) {
		__impl::remove_pointer(ptr);
		ptr = cpy.ptr;
		object_size = cpy.object_size;
		
		__impl::register_pointer(ptr, object_size);

		return *this;
	}
	
	void *raw_tracker::get_raw_pointer() const {
		return ptr;
	}
	
	usize raw_tracker::get_size() const {
		return object_size;
	}
} // namespace uldl
