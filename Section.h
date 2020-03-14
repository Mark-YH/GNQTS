//
// Created by Lab114 on 2020/3/14.
//
#ifndef GNQTS_STOCK_SECTION_H
#define GNQTS_STOCK_SECTION_H

#include <string>

#define SECTION 0
/** @param SECTION
 *  0: M2M
 *  1:
 *
 */

using std::string;

#if SECTION == 0
const string tag = "M2M";
const int numOfSection = 96;
const string section_train[] = {
        "train_2009_12(2009 Q1).csv", "train_2010_01(2010 Q1).csv", "train_2010_02(2010 Q1).csv",
        "train_2010_03(2010 Q1).csv", "train_2010_04(2010 Q1).csv", "train_2010_05(2010 Q1).csv",
        "train_2010_06(2010 Q1).csv", "train_2010_07(2010 Q1).csv", "train_2010_08(2010 Q1).csv",
        "train_2010_09(2010 Q1).csv", "train_2010_10(2010 Q1).csv", "train_2010_11(2010 Q1).csv",
        "train_2010_12(2010 Q1).csv", "train_2011_01(2011 Q1).csv", "train_2011_02(2011 Q1).csv",
        "train_2011_03(2011 Q1).csv", "train_2011_04(2011 Q1).csv", "train_2011_05(2011 Q1).csv",
        "train_2011_06(2011 Q1).csv", "train_2011_07(2011 Q1).csv", "train_2011_08(2011 Q1).csv",
        "train_2011_09(2011 Q1).csv", "train_2011_10(2011 Q1).csv", "train_2011_11(2011 Q1).csv",
        "train_2011_12(2011 Q1).csv", "train_2012_01(2012 Q1).csv", "train_2012_02(2012 Q1).csv",
        "train_2012_03(2012 Q1).csv", "train_2012_04(2012 Q1).csv", "train_2012_05(2012 Q1).csv",
        "train_2012_06(2012 Q1).csv", "train_2012_07(2012 Q1).csv", "train_2012_08(2012 Q1).csv",
        "train_2012_09(2012 Q1).csv", "train_2012_10(2012 Q1).csv", "train_2012_11(2012 Q1).csv",
        "train_2012_12(2012 Q1).csv", "train_2013_01(2013 Q1).csv", "train_2013_02(2013 Q1).csv",
        "train_2013_03(2013 Q1).csv", "train_2013_04(2013 Q1).csv", "train_2013_05(2013 Q1).csv",
        "train_2013_06(2013 Q1).csv", "train_2013_07(2013 Q1).csv", "train_2013_08(2013 Q1).csv",
        "train_2013_09(2013 Q1).csv", "train_2013_10(2013 Q1).csv", "train_2013_11(2013 Q1).csv",
        "train_2013_12(2013 Q1).csv", "train_2014_01(2014 Q1).csv", "train_2014_02(2014 Q1).csv",
        "train_2014_03(2014 Q1).csv", "train_2014_04(2014 Q1).csv", "train_2014_05(2014 Q1).csv",
        "train_2014_06(2014 Q1).csv", "train_2014_07(2014 Q1).csv", "train_2014_08(2014 Q1).csv",
        "train_2014_09(2014 Q1).csv", "train_2014_10(2014 Q1).csv", "train_2014_11(2014 Q1).csv",
        "train_2014_12(2014 Q1).csv", "train_2015_01(2015 Q1).csv", "train_2015_02(2015 Q1).csv",
        "train_2015_03(2015 Q1).csv", "train_2015_04(2015 Q1).csv", "train_2015_05(2015 Q1).csv",
        "train_2015_06(2015 Q1).csv", "train_2015_07(2015 Q1).csv", "train_2015_08(2015 Q1).csv",
        "train_2015_09(2015 Q1).csv", "train_2015_10(2015 Q1).csv", "train_2015_11(2015 Q1).csv",
        "train_2015_12(2015 Q1).csv", "train_2016_01(2016 Q1).csv", "train_2016_02(2016 Q1).csv",
        "train_2016_03(2016 Q1).csv", "train_2016_04(2016 Q1).csv", "train_2016_05(2016 Q1).csv",
        "train_2016_06(2016 Q1).csv", "train_2016_07(2016 Q1).csv", "train_2016_08(2016 Q1).csv",
        "train_2016_09(2016 Q1).csv", "train_2016_10(2016 Q1).csv", "train_2016_11(2016 Q1).csv",
        "train_2016_12(2016 Q1).csv", "train_2017_01(2017 Q1).csv", "train_2017_02(2017 Q1).csv",
        "train_2017_03(2017 Q1).csv", "train_2017_04(2017 Q1).csv", "train_2017_05(2017 Q1).csv",
        "train_2017_06(2017 Q1).csv", "train_2017_07(2017 Q1).csv", "train_2017_08(2017 Q1).csv",
        "train_2017_09(2017 Q1).csv", "train_2017_10(2017 Q1).csv", "train_2017_11(2017 Q1).csv",
        "train_2017_12(2017 Q1).csv"};
#endif

#endif //GNQTS_STOCK_SECTION_H
