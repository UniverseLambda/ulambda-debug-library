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

#include <uldl.hpp>
#include <uldl_rt.hpp>
#include <cstdlib>

#include <execinfo.h>
#include <unistd.h>

#include <iostream>

namespace uldl {

namespace __impl {
	usize global_counter = 0;
	tracker_data *trackers = NULL;
	tracker_data *trackers_end = NULL;

	static void print_stacktrace(void **stacktrace, usize backtrace_count) {
		char **backtrace = backtrace_symbols(stacktrace + ULDL_BACKTRACE_OFFSET, backtrace_count);
		if (backtrace != NULL) {
			for (usize i = 0; i < (backtrace_count - ULDL_BACKTRACE_OFFSET); ++i) {
				std::cerr << "\t> " << backtrace[i] << std::endl;
			}
			std::free(backtrace);
		} else {
			backtrace_symbols_fd(stacktrace + ULDL_BACKTRACE_OFFSET, backtrace_count - ULDL_BACKTRACE_OFFSET, STDERR_FILENO);
		}
	}

	static void print_tracker_data(tracker_data *data) {
		std::cerr << "\tpointer    : " << data->ptr << std::endl;
		std::cerr << "\tobject size: " << data->size << std::endl;
		std::cerr << "\ttracker id : " << data->tracker_id << std::endl;
		std::cerr << "\ttracked at : " << std::endl;

		print_stacktrace(data->ctrct_backtrace, data->ctrct_backtrace_size);
	}

	static void cleanup_and_report() {
		usize stale_count = 0;

		std::cerr << "======= ULDL REPORT =======" << std::endl;
		for (tracker_data *curr = trackers; curr != NULL;) {
			std::cerr << "=> Stale object <=" << std::endl;
			print_tracker_data(curr);

			tracker_data *old = curr;
			curr = curr->next;
			delete old;

			++stale_count;
		}

		if (stale_count > 0) {
			std::cerr << "--------------------------" << std::endl;
			std::cerr << "found " << stale_count << " object" << ((stale_count > 1) ? "s" : "") << std::endl;
			std::cerr << "Better luck next time! (those devs...)" << std::endl;
		} else {
			std::cerr << "no stale object found. Nice :) (jk, you just need to track the right objects. There's ALWAYS a problem. You just don't know yet)" << std::endl;
		}
	}

	void register_pointer(void *ptr, usize type_size) {
		char *no_dup_env = getenv("ULDL_NO_DUP");
		
		if (no_dup_env != NULL && no_dup_env[0] == '1' && no_dup_env[1] == '\0') {
			for (tracker_data *curr = trackers; curr != NULL; curr = curr->next) {
				if (curr->ptr == ptr) {
					std::cerr << "===== ULDL - ERROR - found duplicate of object ===== " << std::endl;
					std::cerr << "=> Duplicate object <=" << std::endl;
					print_tracker_data(curr);

					void *backtrace_sym[ULDL_BACKTRACE_SIZE + ULDL_BACKTRACE_OFFSET];
					usize backtrace_count = backtrace(backtrace_sym, ULDL_BACKTRACE_SIZE + ULDL_BACKTRACE_OFFSET);

					std::cerr << "\tDuplicate stacktrace: " << std::endl;
					print_stacktrace(backtrace_sym, backtrace_count);
					std::abort();
				}
			}
		}


		tracker_data *data = new tracker_data();

		// std::cerr << "Registering object " << ptr << " of size " << type_size << std::endl;

		data->ptr = ptr;
		data->size = type_size;
		data->tracker_id = global_counter++;


		data->ctrct_backtrace_size = backtrace(data->ctrct_backtrace, ULDL_BACKTRACE_SIZE);

		data->next = NULL;
		if (trackers == NULL) {
			data->previous = NULL;
			trackers = data;
			trackers_end = data;
		} else {
			trackers_end->next = data;
			data->previous = trackers_end;
			trackers_end = data;
		}
	}

	void remove_pointer(void *ptr) {
		// std::cerr << "Removing object " << ptr << std::endl;
		for (tracker_data *curr = trackers; curr != NULL; curr = curr->next) {
			if (curr->ptr == ptr) {
				if (curr->previous != NULL) {
					curr->previous->next = curr->next;
				} else {
					trackers = curr->next;
				}

				if (curr->next != NULL) {
					curr->next->previous = curr->previous;
				} else {
					trackers_end = curr->previous;
				}

				delete curr;
				return;
			}
		}
		std::cerr << "uldl: warning: object " << ptr << " not tracked" << std::endl;
	}
} // namespace __impl
	void init() {
		if (std::atexit(__impl::cleanup_and_report) != 0) {
			std::cerr << "uldl: call to atexit failed" << std::endl;
		}
	}
} // namespace uldl
