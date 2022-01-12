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

#ifndef ULDL_BACKTRACE_SIZE
# define ULDL_BACKTRACE_SIZE 20
#endif // ifndef ULDL_BACKTRACE_SIZE

#define ULDL_BACKTRACE_OFFSET 3

#include <uldl.hpp>

namespace uldl {
namespace __impl {
	struct tracker_data {
		void *ptr;
		usize size;
		
		usize tracker_id;
		void *ctrct_backtrace[ULDL_BACKTRACE_SIZE + ULDL_BACKTRACE_OFFSET];
		usize ctrct_backtrace_size;

		tracker_data *previous;
		tracker_data *next;
	};
	
	void register_pointer(void *ptr, usize type_size);
	void remove_pointer(void *ptr);
} // namespace __impl
} // namespace uldl

