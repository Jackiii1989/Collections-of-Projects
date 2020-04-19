//           $Id: thread.cpp 37984 2018-10-27 15:50:30Z p20068 $
//          $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/vocational/teaching/ESD/SPS3/2018-WS/ILV/src/Snippets/thread.cpp $
//     $Revision: 37984 $
//         $Date: 2018-10-27 17:50:30 +0200 (Sa., 27 Okt 2018) $
//       $Author: p20068 $
//
//       Creator: Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
// Creation Date:
//     Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//
//       License: This document contains proprietary information belonging to
//                University of Applied Sciences Upper Austria, Campus
//                Hagenberg. It is distributed under the Boost Software License,
//                Version 1.0 (see http://www.boost.org/LICENSE_1_0.txt).
//
//    Annotation: This file is part of the code snippets handed out during one
//                of my HPC lessons held at the University of Applied Sciences
//                Upper Austria, Campus Hagenberg.
#pragma once
#include <vector>
#include <future>
class task_group final {
public:
	explicit task_group() = default;

	task_group(task_group const &) = delete;
	task_group(task_group &&) = default;

	~task_group() {
		join_all();
	}

	task_group & operator = (task_group const &) = delete;
	task_group & operator = (task_group &&) = default;

	template <typename fun_t, typename ...args_t> void add(fun_t && fun, args_t && ...args) {
		m_group.push_back(
			std::async(std::launch::async, std::forward <fun_t>(fun), std::forward <args_t>(args)...)
		);
	}

	void join_all() {
		for (auto & f : m_group) f.wait();
	}

private:
	std::vector <std::future <void>> m_group;
};

class thread_group final {
public:
	explicit thread_group() = default;

	thread_group(thread_group const &) = delete;
	thread_group(thread_group &&) = default;

	~thread_group() {
		join_all();
	}

	thread_group & operator = (thread_group const &) = delete;
	thread_group & operator = (thread_group &&) = default;

	template <typename fun_t, typename ...args_t> void add(fun_t && fun, args_t && ...args) {
		m_group.emplace_back(std::forward <fun_t>(fun), std::forward <args_t>(args)...);
	}

	void join_all() {
		for (auto & t : m_group) if (t.joinable()) t.join();
	}

private:
	std::vector <std::thread> m_group;
};