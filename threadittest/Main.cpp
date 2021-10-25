/*-------------------------------------------------------------------------*/
/* Copyright (C) 2021 by Ashkel Software                                   */
/* ari@ashkel.com.au                                                       */
/*                                                                         */
/* This file is part of the threadit library.                              */
/*                                                                         */
/* The threadit library is free software; you can redistribute it and/or   */
/* modify it under the terms of The Code Project Open License (CPOL) 1.02  */
/*                                                                         */
/* The threadit library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the CPOL       */
/* License for more details.                                               */
/*                                                                         */
/* You should have received a copy of the CPOL License along with this     */
/* software.                                                               */
/*-------------------------------------------------------------------------*/

#include <string>
#include "UnitTest++.h"
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"

int main(int, char const* [])
{
	log4cpp::Appender* appender;

	

	appender = new log4cpp::FileAppender("default", "../log/test.log");
	appender->setLayout(new log4cpp::BasicLayout());
	log4cpp::Category& root = log4cpp::Category::getRoot();

	log4cpp::PatternLayout* ostreamLayout = new log4cpp::PatternLayout();
	ostreamLayout->setConversionPattern("%d: %p %c %x: %m %n");
	log4cpp::Appender* ostreamAppender = new log4cpp::OstreamAppender("ostreamAppender", &std::cout);
	ostreamAppender->setLayout(ostreamLayout);

	root.addAppender(appender);
	root.addAppender(ostreamAppender);
	root.setPriority(log4cpp::Priority::INFO);

	std::cout << " root prio = " << root.getPriority() << std::endl;

	return UnitTest::RunAllTests();
}