//
// Created by Mark Hsu on 2020/3/14.
//
#ifndef GNQTS_STOCK_SLIDINGWINDOW_H
#define GNQTS_STOCK_SLIDINGWINDOW_H

#include <string>

#define WINDOW 0
/** @param WINDOW
 *   0: M2M
 *   1: Y2Y
 *   2: Y2Q
 *   3: Y2M
 *   4: Y2H
 *   5: Q2Q
 *   6: Q2M
 *   7: H2H
 *   8: H2Q
 *   9: H2M
 *  10: H*
 *  11: M*
 *  12: Q*
 */

using std::string;

#if WINDOW == 0
const string tag = "M2M";
const int numOfSection = 96;
const string trainingSection[] = {
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

#elif WINDOW == 1
const string tag = "Y2Y";
const int numOfSection = 8;
const string trainingSection[] = {
        "train_2009(2009 Q1).csv", "train_2010(2010 Q1).csv", "train_2011(2011 Q1).csv",
        "train_2012(2012 Q1).csv", "train_2013(2013 Q1).csv", "train_2014(2014 Q1).csv",
        "train_2015(2015 Q1).csv", "train_2016(2016 Q1).csv"};;

#elif WINDOW == 2
const string tag = "Y2Q";
const int numOfSection = 32;
const string trainingSection[] = {
        "train_2009(2009 Q1).csv", "train_2009_Q2~2010_Q1(2009 Q1).csv",
        "train_2009_Q3~2010_Q2(2009 Q1).csv", "train_2009_Q4~2010_Q3(2009 Q1).csv",
        "train_2010(2010 Q1).csv", "train_2010_Q2~2011_Q1(2010 Q1).csv",
        "train_2010_Q3~2011_Q2(2010 Q1).csv", "train_2010_Q4~2011_Q3(2010 Q1).csv",
        "train_2011(2011 Q1).csv", "train_2011_Q2~2012_Q1(2011 Q1).csv",
        "train_2011_Q3~2012_Q2(2011 Q1).csv", "train_2011_Q4~2012_Q3(2011 Q1).csv",
        "train_2012(2012 Q1).csv", "train_2012_Q2~2013_Q1(2012 Q1).csv",
        "train_2012_Q3~2013_Q2(2012 Q1).csv", "train_2012_Q4~2013_Q3(2012 Q1).csv",
        "train_2013(2013 Q1).csv", "train_2013_Q2~2014_Q1(2013 Q1).csv",
        "train_2013_Q3~2014_Q2(2013 Q1).csv", "train_2013_Q4~2014_Q3(2013 Q1).csv",
        "train_2014(2014 Q1).csv", "train_2014_Q2~2015_Q1(2014 Q1).csv",
        "train_2014_Q3~2015_Q2(2014 Q1).csv", "train_2014_Q4~2015_Q3(2014 Q1).csv",
        "train_2015(2015 Q1).csv", "train_2015_Q2~2016_Q1(2015 Q1).csv",
        "train_2015_Q3~2016_Q2(2015 Q1).csv", "train_2015_Q4~2016_Q3(2015 Q1).csv",
        "train_2016(2016 Q1).csv", "train_2016_Q2~2017_Q1(2016 Q1).csv",
        "train_2016_Q3~2017_Q2(2016 Q1).csv", "train_2016_Q4~2017_Q3(2016 Q1).csv"};
#elif WINDOW == 3
const string tag = "Y2M";
const int numOfSection = 96;
const string trainingSection[] = {
        "train_2009(2009 Q1).csv", "train_2009_02~2010_01(2009 Q1).csv",
        "train_2009_03~2010_02(2009 Q1).csv", "train_2009_04~2010_03(2009 Q1).csv",
        "train_2009_05~2010_04(2009 Q1).csv", "train_2009_06~2010_05(2009 Q1).csv",
        "train_2009_07~2010_06(2009 Q1).csv", "train_2009_08~2010_07(2009 Q1).csv",
        "train_2009_09~2010_08(2009 Q1).csv", "train_2009_10~2010_09(2009 Q1).csv",
        "train_2009_11~2010_10(2009 Q1).csv", "train_2009_12~2010_11(2009 Q1).csv",
        "train_2010(2010 Q1).csv", "train_2010_02~2011_01(2010 Q1).csv",
        "train_2010_03~2011_02(2010 Q1).csv", "train_2010_04~2011_03(2010 Q1).csv",
        "train_2010_05~2011_04(2010 Q1).csv", "train_2010_06~2011_05(2010 Q1).csv",
        "train_2010_07~2011_06(2010 Q1).csv", "train_2010_08~2011_07(2010 Q1).csv",
        "train_2010_09~2011_08(2010 Q1).csv", "train_2010_10~2011_09(2010 Q1).csv",
        "train_2010_11~2011_10(2010 Q1).csv", "train_2010_12~2011_11(2010 Q1).csv",
        "train_2011(2011 Q1).csv", "train_2011_02~2012_01(2011 Q1).csv",
        "train_2011_03~2012_02(2011 Q1).csv", "train_2011_04~2012_03(2011 Q1).csv",
        "train_2011_05~2012_04(2011 Q1).csv", "train_2011_06~2012_05(2011 Q1).csv",
        "train_2011_07~2012_06(2011 Q1).csv", "train_2011_08~2012_07(2011 Q1).csv",
        "train_2011_09~2012_08(2011 Q1).csv", "train_2011_10~2012_09(2011 Q1).csv",
        "train_2011_11~2012_10(2011 Q1).csv", "train_2011_12~2012_11(2011 Q1).csv",
        "train_2012(2012 Q1).csv", "train_2012_02~2013_01(2012 Q1).csv",
        "train_2012_03~2013_02(2012 Q1).csv", "train_2012_04~2013_03(2012 Q1).csv",
        "train_2012_05~2013_04(2012 Q1).csv", "train_2012_06~2013_05(2012 Q1).csv",
        "train_2012_07~2013_06(2012 Q1).csv", "train_2012_08~2013_07(2012 Q1).csv",
        "train_2012_09~2013_08(2012 Q1).csv", "train_2012_10~2013_09(2012 Q1).csv",
        "train_2012_11~2013_10(2012 Q1).csv", "train_2012_12~2013_11(2012 Q1).csv",
        "train_2013(2013 Q1).csv", "train_2013_02~2014_01(2013 Q1).csv",
        "train_2013_03~2014_02(2013 Q1).csv", "train_2013_04~2014_03(2013 Q1).csv",
        "train_2013_05~2014_04(2013 Q1).csv", "train_2013_06~2014_05(2013 Q1).csv",
        "train_2013_07~2014_06(2013 Q1).csv", "train_2013_08~2014_07(2013 Q1).csv",
        "train_2013_09~2014_08(2013 Q1).csv", "train_2013_10~2014_09(2013 Q1).csv",
        "train_2013_11~2014_10(2013 Q1).csv", "train_2013_12~2014_11(2013 Q1).csv",
        "train_2014(2014 Q1).csv", "train_2014_02~2015_01(2014 Q1).csv",
        "train_2014_03~2015_02(2014 Q1).csv", "train_2014_04~2015_03(2014 Q1).csv",
        "train_2014_05~2015_04(2014 Q1).csv", "train_2014_06~2015_05(2014 Q1).csv",
        "train_2014_07~2015_06(2014 Q1).csv", "train_2014_08~2015_07(2014 Q1).csv",
        "train_2014_09~2015_08(2014 Q1).csv", "train_2014_10~2015_09(2014 Q1).csv",
        "train_2014_11~2015_10(2014 Q1).csv", "train_2014_12~2015_11(2014 Q1).csv",
        "train_2015(2015 Q1).csv", "train_2015_02~2016_01(2015 Q1).csv",
        "train_2015_03~2016_02(2015 Q1).csv", "train_2015_04~2016_03(2015 Q1).csv",
        "train_2015_05~2016_04(2015 Q1).csv", "train_2015_06~2016_05(2015 Q1).csv",
        "train_2015_07~2016_06(2015 Q1).csv", "train_2015_08~2016_07(2015 Q1).csv",
        "train_2015_09~2016_08(2015 Q1).csv", "train_2015_10~2016_09(2015 Q1).csv",
        "train_2015_11~2016_10(2015 Q1).csv", "train_2015_12~2016_11(2015 Q1).csv",
        "train_2016(2016 Q1).csv", "train_2016_02~2017_01(2016 Q1).csv",
        "train_2016_03~2017_02(2016 Q1).csv", "train_2016_04~2017_03(2016 Q1).csv",
        "train_2016_05~2017_04(2016 Q1).csv", "train_2016_06~2017_05(2016 Q1).csv",
        "train_2016_07~2017_06(2016 Q1).csv", "train_2016_08~2017_07(2016 Q1).csv",
        "train_2016_09~2017_08(2016 Q1).csv", "train_2016_10~2017_09(2016 Q1).csv",
        "train_2016_11~2017_10(2016 Q1).csv", "train_2016_12~2017_11(2016 Q1).csv"};

#elif WINDOW == 4
const string tag = "Y2H";
const int numOfSection = 16;
const string trainingSection[] = {
        "train_2009(2009 Q1).csv", "train_2009_Q3~2010_Q2(2009 Q1).csv", "train_2010(2010 Q1).csv",
        "train_2010_Q3~2011_Q2(2010 Q1).csv", "train_2011(2011 Q1).csv", "train_2011_Q3~2012_Q2(2011 Q1).csv",
        "train_2012(2012 Q1).csv", "train_2012_Q3~2013_Q2(2012 Q1).csv", "train_2013(2013 Q1).csv",
        "train_2013_Q3~2014_Q2(2013 Q1).csv", "train_2014(2014 Q1).csv", "train_2014_Q3~2015_Q2(2014 Q1).csv",
        "train_2015(2015 Q1).csv", "train_2015_Q3~2016_Q2(2015 Q1).csv", "train_2016(2016 Q1).csv",
        "train_2016_Q3~2017_Q2(2016 Q1).csv"};

#elif WINDOW == 5
const string tag = "Q2Q";
const int numOfSection = 32;
const string trainingSection[] = {
        "train_2009_Q4(2009 Q1).csv", "train_2010_Q1(2010 Q1).csv", "train_2010_Q2(2010 Q1).csv",
        "train_2010_Q3(2010 Q1).csv", "train_2010_Q4(2010 Q1).csv", "train_2011_Q1(2011 Q1).csv",
        "train_2011_Q2(2011 Q1).csv", "train_2011_Q3(2011 Q1).csv", "train_2011_Q4(2011 Q1).csv",
        "train_2012_Q1(2012 Q1).csv", "train_2012_Q2(2012 Q1).csv", "train_2012_Q3(2012 Q1).csv",
        "train_2012_Q4(2012 Q1).csv", "train_2013_Q1(2013 Q1).csv", "train_2013_Q2(2013 Q1).csv",
        "train_2013_Q3(2013 Q1).csv", "train_2013_Q4(2013 Q1).csv", "train_2014_Q1(2014 Q1).csv",
        "train_2014_Q2(2014 Q1).csv", "train_2014_Q3(2014 Q1).csv", "train_2014_Q4(2014 Q1).csv",
        "train_2015_Q1(2015 Q1).csv", "train_2015_Q2(2015 Q1).csv", "train_2015_Q3(2015 Q1).csv",
        "train_2015_Q4(2015 Q1).csv", "train_2016_Q1(2016 Q1).csv", "train_2016_Q2(2016 Q1).csv",
        "train_2016_Q3(2016 Q1).csv", "train_2016_Q4(2016 Q1).csv", "train_2017_Q1(2017 Q1).csv",
        "train_2017_Q2(2017 Q1).csv", "train_2017_Q3(2017 Q1).csv"};

#elif WINDOW == 6
const string tag = "Q2M";
const int numOfSection = 96;
const string trainingSection[] = {
        "train_2009_10-12(2009 Q1).csv", "train_2009_11~2010_01(2009 Q1).csv", "train_2009_12~2010_02(2009 Q1).csv",
        "train_2010_01-03(2010 Q1).csv", "train_2010_02-04(2010 Q1).csv", "train_2010_03-05(2010 Q1).csv",
        "train_2010_04-06(2010 Q1).csv", "train_2010_05-07(2010 Q1).csv", "train_2010_06-08(2010 Q1).csv",
        "train_2010_07-09(2010 Q1).csv", "train_2010_08-10(2010 Q1).csv", "train_2010_09-11(2010 Q1).csv",
        "train_2010_10-12(2010 Q1).csv", "train_2010_11~2011_01(2010 Q1).csv", "train_2010_12~2011_02(2010 Q1).csv",
        "train_2011_01-03(2011 Q1).csv", "train_2011_02-04(2011 Q1).csv", "train_2011_03-05(2011 Q1).csv",
        "train_2011_04-06(2011 Q1).csv", "train_2011_05-07(2011 Q1).csv", "train_2011_06-08(2011 Q1).csv",
        "train_2011_07-09(2011 Q1).csv", "train_2011_08-10(2011 Q1).csv", "train_2011_09-11(2011 Q1).csv",
        "train_2011_10-12(2011 Q1).csv", "train_2011_11~2012_01(2011 Q1).csv", "train_2011_12~2012_02(2011 Q1).csv",
        "train_2012_01-03(2012 Q1).csv", "train_2012_02-04(2012 Q1).csv", "train_2012_03-05(2012 Q1).csv",
        "train_2012_04-06(2012 Q1).csv", "train_2012_05-07(2012 Q1).csv", "train_2012_06-08(2012 Q1).csv",
        "train_2012_07-09(2012 Q1).csv", "train_2012_08-10(2012 Q1).csv", "train_2012_09-11(2012 Q1).csv",
        "train_2012_10-12(2012 Q1).csv", "train_2012_11~2013_01(2012 Q1).csv", "train_2012_12~2013_02(2012 Q1).csv",
        "train_2013_01-03(2013 Q1).csv", "train_2013_02-04(2013 Q1).csv", "train_2013_03-05(2013 Q1).csv",
        "train_2013_04-06(2013 Q1).csv", "train_2013_05-07(2013 Q1).csv", "train_2013_06-08(2013 Q1).csv",
        "train_2013_07-09(2013 Q1).csv", "train_2013_08-10(2013 Q1).csv", "train_2013_09-11(2013 Q1).csv",
        "train_2013_10-12(2013 Q1).csv", "train_2013_11~2014_01(2013 Q1).csv", "train_2013_12~2014_02(2013 Q1).csv",
        "train_2014_01-03(2014 Q1).csv", "train_2014_02-04(2014 Q1).csv", "train_2014_03-05(2014 Q1).csv",
        "train_2014_04-06(2014 Q1).csv", "train_2014_05-07(2014 Q1).csv", "train_2014_06-08(2014 Q1).csv",
        "train_2014_07-09(2014 Q1).csv", "train_2014_08-10(2014 Q1).csv", "train_2014_09-11(2014 Q1).csv",
        "train_2014_10-12(2014 Q1).csv", "train_2014_11~2015_01(2014 Q1).csv", "train_2014_12~2015_02(2014 Q1).csv",
        "train_2015_01-03(2015 Q1).csv", "train_2015_02-04(2015 Q1).csv", "train_2015_03-05(2015 Q1).csv",
        "train_2015_04-06(2015 Q1).csv", "train_2015_05-07(2015 Q1).csv", "train_2015_06-08(2015 Q1).csv",
        "train_2015_07-09(2015 Q1).csv", "train_2015_08-10(2015 Q1).csv", "train_2015_09-11(2015 Q1).csv",
        "train_2015_10-12(2015 Q1).csv", "train_2015_11~2016_01(2015 Q1).csv", "train_2015_12~2016_02(2015 Q1).csv",
        "train_2016_01-03(2016 Q1).csv", "train_2016_02-04(2016 Q1).csv", "train_2016_03-05(2016 Q1).csv",
        "train_2016_04-06(2016 Q1).csv", "train_2016_05-07(2016 Q1).csv", "train_2016_06-08(2016 Q1).csv",
        "train_2016_07-09(2016 Q1).csv", "train_2016_08-10(2016 Q1).csv", "train_2016_09-11(2016 Q1).csv",
        "train_2016_10-12(2016 Q1).csv", "train_2016_11~2017_01(2016 Q1).csv", "train_2016_12~2017_02(2016 Q1).csv",
        "train_2017_01-03(2017 Q1).csv", "train_2017_02-04(2017 Q1).csv", "train_2017_03-05(2017 Q1).csv",
        "train_2017_04-06(2017 Q1).csv", "train_2017_05-07(2017 Q1).csv", "train_2017_06-08(2017 Q1).csv",
        "train_2017_07-09(2017 Q1).csv", "train_2017_08-10(2017 Q1).csv", "train_2017_09-11(2017 Q1).csv"};

#elif WINDOW == 7
const string tag = "H2H";
const int numOfSection = 16;
const string trainingSection[] = {
        "train_2009_Q3-Q4(2009 Q1).csv", "train_2010_Q1-Q2(2010 Q1).csv", "train_2010_Q3-Q4(2010 Q1).csv",
        "train_2011_Q1-Q2(2011 Q1).csv", "train_2011_Q3-Q4(2011 Q1).csv", "train_2012_Q1-Q2(2012 Q1).csv",
        "train_2012_Q3-Q4(2012 Q1).csv", "train_2013_Q1-Q2(2013 Q1).csv", "train_2013_Q3-Q4(2013 Q1).csv",
        "train_2014_Q1-Q2(2014 Q1).csv", "train_2014_Q3-Q4(2014 Q1).csv", "train_2015_Q1-Q2(2015 Q1).csv",
        "train_2015_Q3-Q4(2015 Q1).csv", "train_2016_Q1-Q2(2016 Q1).csv", "train_2016_Q3-Q4(2016 Q1).csv",
        "train_2017_Q1-Q2(2017 Q1).csv"};

#elif WINDOW == 8
const string tag = "H2Q";
const int numOfSection = 32;
const string trainingSection[] = {
        "train_2009_Q3-Q4(2009 Q1).csv", "train_2009_Q4~2010_Q1(2009 Q1).csv",
        "train_2010_Q1-Q2(2010 Q1).csv", "train_2010_Q2-Q3(2010 Q1).csv",
        "train_2010_Q3-Q4(2010 Q1).csv", "train_2010_Q4~2011_Q1(2010 Q1).csv",
        "train_2011_Q1-Q2(2011 Q1).csv", "train_2011_Q2-Q3(2011 Q1).csv",
        "train_2011_Q3-Q4(2011 Q1).csv", "train_2011_Q4~2012_Q1(2011 Q1).csv",
        "train_2012_Q1-Q2(2012 Q1).csv", "train_2012_Q2-Q3(2012 Q1).csv",
        "train_2012_Q3-Q4(2012 Q1).csv", "train_2012_Q4~2013_Q1(2012 Q1).csv",
        "train_2013_Q1-Q2(2013 Q1).csv", "train_2013_Q2-Q3(2013 Q1).csv",
        "train_2013_Q3-Q4(2013 Q1).csv", "train_2013_Q4~2014_Q1(2013 Q1).csv",
        "train_2014_Q1-Q2(2014 Q1).csv", "train_2014_Q2-Q3(2014 Q1).csv",
        "train_2014_Q3-Q4(2014 Q1).csv", "train_2014_Q4~2015_Q1(2014 Q1).csv",
        "train_2015_Q1-Q2(2015 Q1).csv", "train_2015_Q2-Q3(2015 Q1).csv",
        "train_2015_Q3-Q4(2015 Q1).csv", "train_2015_Q4~2016_Q1(2015 Q1).csv",
        "train_2016_Q1-Q2(2016 Q1).csv", "train_2016_Q2-Q3(2016 Q1).csv",
        "train_2016_Q3-Q4(2016 Q1).csv", "train_2016_Q4~2017_Q1(2016 Q1).csv",
        "train_2017_Q1-Q2(2017 Q1).csv", "train_2017_Q2-Q3(2017 Q1).csv"};

#elif WINDOW == 9
const string tag = "H2M";
const int numOfSection = 96;
const string trainingSection[] = {
        "train_2009_07-12(2009 Q1).csv", "train_2009_08~2010_01(2009 Q1).csv", "train_2009_09~2010_02(2009 Q1).csv",
        "train_2009_10~2010_03(2009 Q1).csv", "train_2009_11~2010_04(2009 Q1).csv",
        "train_2009_12~2010_05(2009 Q1).csv", "train_2010_01-06(2010 Q1).csv", "train_2010_02-07(2010 Q1).csv",
        "train_2010_03-08(2010 Q1).csv", "train_2010_04-09(2010 Q1).csv", "train_2010_05-10(2010 Q1).csv",
        "train_2010_06-11(2010 Q1).csv", "train_2010_07-12(2010 Q1).csv", "train_2010_08~2011_01(2010 Q1).csv",
        "train_2010_09~2011_02(2010 Q1).csv", "train_2010_10~2011_03(2010 Q1).csv",
        "train_2010_11~2011_04(2010 Q1).csv", "train_2010_12~2011_05(2010 Q1).csv", "train_2011_01-06(2011 Q1).csv",
        "train_2011_02-07(2011 Q1).csv", "train_2011_03-08(2011 Q1).csv", "train_2011_04-09(2011 Q1).csv",
        "train_2011_05-10(2011 Q1).csv", "train_2011_06-11(2011 Q1).csv", "train_2011_07-12(2011 Q1).csv",
        "train_2011_08~2012_01(2011 Q1).csv", "train_2011_09~2012_02(2011 Q1).csv",
        "train_2011_10~2012_03(2011 Q1).csv", "train_2011_11~2012_04(2011 Q1).csv",
        "train_2011_12~2012_05(2011 Q1).csv", "train_2012_01-06(2012 Q1).csv", "train_2012_02-07(2012 Q1).csv",
        "train_2012_03-08(2012 Q1).csv", "train_2012_04-09(2012 Q1).csv", "train_2012_05-10(2012 Q1).csv",
        "train_2012_06-11(2012 Q1).csv", "train_2012_07-12(2012 Q1).csv", "train_2012_08~2013_01(2012 Q1).csv",
        "train_2012_09~2013_02(2012 Q1).csv", "train_2012_10~2013_03(2012 Q1).csv",
        "train_2012_11~2013_04(2012 Q1).csv", "train_2012_12~2013_05(2012 Q1).csv", "train_2013_01-06(2013 Q1).csv",
        "train_2013_02-07(2013 Q1).csv", "train_2013_03-08(2013 Q1).csv", "train_2013_04-09(2013 Q1).csv",
        "train_2013_05-10(2013 Q1).csv", "train_2013_06-11(2013 Q1).csv", "train_2013_07-12(2013 Q1).csv",
        "train_2013_08~2014_01(2013 Q1).csv", "train_2013_09~2014_02(2013 Q1).csv",
        "train_2013_10~2014_03(2013 Q1).csv", "train_2013_11~2014_04(2013 Q1).csv",
        "train_2013_12~2014_05(2013 Q1).csv", "train_2014_01-06(2014 Q1).csv", "train_2014_02-07(2014 Q1).csv",
        "train_2014_03-08(2014 Q1).csv", "train_2014_04-09(2014 Q1).csv", "train_2014_05-10(2014 Q1).csv",
        "train_2014_06-11(2014 Q1).csv", "train_2014_07-12(2014 Q1).csv", "train_2014_08~2015_01(2014 Q1).csv",
        "train_2014_09~2015_02(2014 Q1).csv", "train_2014_10~2015_03(2014 Q1).csv",
        "train_2014_11~2015_04(2014 Q1).csv", "train_2014_12~2015_05(2014 Q1).csv", "train_2015_01-06(2015 Q1).csv",
        "train_2015_02-07(2015 Q1).csv", "train_2015_03-08(2015 Q1).csv", "train_2015_04-09(2015 Q1).csv",
        "train_2015_05-10(2015 Q1).csv", "train_2015_06-11(2015 Q1).csv", "train_2015_07-12(2015 Q1).csv",
        "train_2015_08~2016_01(2015 Q1).csv", "train_2015_09~2016_02(2015 Q1).csv",
        "train_2015_10~2016_03(2015 Q1).csv", "train_2015_11~2016_04(2015 Q1).csv",
        "train_2015_12~2016_05(2015 Q1).csv", "train_2016_01-06(2016 Q1).csv", "train_2016_02-07(2016 Q1).csv",
        "train_2016_03-08(2016 Q1).csv", "train_2016_04-09(2016 Q1).csv", "train_2016_05-10(2016 Q1).csv",
        "train_2016_06-11(2016 Q1).csv", "train_2016_07-12(2016 Q1).csv", "train_2016_08~2017_01(2016 Q1).csv",
        "train_2016_09~2017_02(2016 Q1).csv", "train_2016_10~2017_03(2016 Q1).csv",
        "train_2016_11~2017_04(2016 Q1).csv", "train_2016_12~2017_05(2016 Q1).csv", "train_2017_01-06(2017 Q1).csv",
        "train_2017_02-07(2017 Q1).csv", "train_2017_03-08(2017 Q1).csv", "train_2017_04-09(2017 Q1).csv",
        "train_2017_05-10(2017 Q1).csv", "train_2017_06-11(2017 Q1).csv"};

#elif WINDOW == 10
const string tag = "H#";
const int numOfSection = 16;
const string trainingSection[] = {
        "train_2009_Q1-Q2(2009 Q1).csv", "train_2009_Q3-Q4(2009 Q1).csv", "train_2010_Q1-Q2(2010 Q1).csv",
        "train_2010_Q3-Q4(2010 Q1).csv", "train_2011_Q1-Q2(2011 Q1).csv", "train_2011_Q3-Q4(2011 Q1).csv",
        "train_2012_Q1-Q2(2012 Q1).csv", "train_2012_Q3-Q4(2012 Q1).csv", "train_2013_Q1-Q2(2013 Q1).csv",
        "train_2013_Q3-Q4(2013 Q1).csv", "train_2014_Q1-Q2(2014 Q1).csv", "train_2014_Q3-Q4(2014 Q1).csv",
        "train_2015_Q1-Q2(2015 Q1).csv", "train_2015_Q3-Q4(2015 Q1).csv", "train_2016_Q1-Q2(2016 Q1).csv",
        "train_2016_Q3-Q4(2016 Q1).csv"};

#elif WINDOW == 11
const string tag = "M#";
const int numOfSection = 96;
const string trainingSection[] = {
        "train_2009_01(2009 Q1).csv", "train_2009_02(2009 Q1).csv", "train_2009_03(2009 Q1).csv",
        "train_2009_04(2009 Q1).csv", "train_2009_05(2009 Q1).csv", "train_2009_06(2009 Q1).csv",
        "train_2009_07(2009 Q1).csv", "train_2009_08(2009 Q1).csv", "train_2009_09(2009 Q1).csv",
        "train_2009_10(2009 Q1).csv", "train_2009_11(2009 Q1).csv", "train_2009_12(2009 Q1).csv",
        "train_2010_01(2010 Q1).csv", "train_2010_02(2010 Q1).csv", "train_2010_03(2010 Q1).csv",
        "train_2010_04(2010 Q1).csv", "train_2010_05(2010 Q1).csv", "train_2010_06(2010 Q1).csv",
        "train_2010_07(2010 Q1).csv", "train_2010_08(2010 Q1).csv", "train_2010_09(2010 Q1).csv",
        "train_2010_10(2010 Q1).csv", "train_2010_11(2010 Q1).csv", "train_2010_12(2010 Q1).csv",
        "train_2011_01(2011 Q1).csv", "train_2011_02(2011 Q1).csv", "train_2011_03(2011 Q1).csv",
        "train_2011_04(2011 Q1).csv", "train_2011_05(2011 Q1).csv", "train_2011_06(2011 Q1).csv",
        "train_2011_07(2011 Q1).csv", "train_2011_08(2011 Q1).csv", "train_2011_09(2011 Q1).csv",
        "train_2011_10(2011 Q1).csv", "train_2011_11(2011 Q1).csv", "train_2011_12(2011 Q1).csv",
        "train_2012_01(2012 Q1).csv", "train_2012_02(2012 Q1).csv", "train_2012_03(2012 Q1).csv",
        "train_2012_04(2012 Q1).csv", "train_2012_05(2012 Q1).csv", "train_2012_06(2012 Q1).csv",
        "train_2012_07(2012 Q1).csv", "train_2012_08(2012 Q1).csv", "train_2012_09(2012 Q1).csv",
        "train_2012_10(2012 Q1).csv", "train_2012_11(2012 Q1).csv", "train_2012_12(2012 Q1).csv",
        "train_2013_01(2013 Q1).csv", "train_2013_02(2013 Q1).csv", "train_2013_03(2013 Q1).csv",
        "train_2013_04(2013 Q1).csv", "train_2013_05(2013 Q1).csv", "train_2013_06(2013 Q1).csv",
        "train_2013_07(2013 Q1).csv", "train_2013_08(2013 Q1).csv", "train_2013_09(2013 Q1).csv",
        "train_2013_10(2013 Q1).csv", "train_2013_11(2013 Q1).csv", "train_2013_12(2013 Q1).csv",
        "train_2014_01(2014 Q1).csv", "train_2014_02(2014 Q1).csv", "train_2014_03(2014 Q1).csv",
        "train_2014_04(2014 Q1).csv", "train_2014_05(2014 Q1).csv", "train_2014_06(2014 Q1).csv",
        "train_2014_07(2014 Q1).csv", "train_2014_08(2014 Q1).csv", "train_2014_09(2014 Q1).csv",
        "train_2014_10(2014 Q1).csv", "train_2014_11(2014 Q1).csv", "train_2014_12(2014 Q1).csv",
        "train_2015_01(2015 Q1).csv", "train_2015_02(2015 Q1).csv", "train_2015_03(2015 Q1).csv",
        "train_2015_04(2015 Q1).csv", "train_2015_05(2015 Q1).csv", "train_2015_06(2015 Q1).csv",
        "train_2015_07(2015 Q1).csv", "train_2015_08(2015 Q1).csv", "train_2015_09(2015 Q1).csv",
        "train_2015_10(2015 Q1).csv", "train_2015_11(2015 Q1).csv", "train_2015_12(2015 Q1).csv",
        "train_2016_01(2016 Q1).csv", "train_2016_02(2016 Q1).csv", "train_2016_03(2016 Q1).csv",
        "train_2016_04(2016 Q1).csv", "train_2016_05(2016 Q1).csv", "train_2016_06(2016 Q1).csv",
        "train_2016_07(2016 Q1).csv", "train_2016_08(2016 Q1).csv", "train_2016_09(2016 Q1).csv",
        "train_2016_10(2016 Q1).csv", "train_2016_11(2016 Q1).csv", "train_2016_12(2016 Q1).csv"};

#elif WINDOW == 12
const string tag = "Q#";
const int numOfSection = 32;
const string trainingSection[] = {
        "train_2009_Q1(2009 Q1).csv", "train_2009_Q2(2009 Q1).csv",
        "train_2009_Q3(2009 Q1).csv", "train_2009_Q4(2009 Q1).csv",
        "train_2010_Q1(2010 Q1).csv", "train_2010_Q2(2010 Q1).csv",
        "train_2010_Q3(2010 Q1).csv", "train_2010_Q4(2010 Q1).csv",
        "train_2011_Q1(2011 Q1).csv", "train_2011_Q2(2011 Q1).csv",
        "train_2011_Q3(2011 Q1).csv", "train_2011_Q4(2011 Q1).csv",
        "train_2012_Q1(2012 Q1).csv", "train_2012_Q2(2012 Q1).csv",
        "train_2012_Q3(2012 Q1).csv", "train_2012_Q4(2012 Q1).csv",
        "train_2013_Q1(2013 Q1).csv", "train_2013_Q2(2013 Q1).csv",
        "train_2013_Q3(2013 Q1).csv", "train_2013_Q4(2013 Q1).csv",
        "train_2014_Q1(2014 Q1).csv", "train_2014_Q2(2014 Q1).csv",
        "train_2014_Q3(2014 Q1).csv", "train_2014_Q4(2014 Q1).csv",
        "train_2015_Q1(2015 Q1).csv", "train_2015_Q2(2015 Q1).csv",
        "train_2015_Q3(2015 Q1).csv", "train_2015_Q4(2015 Q1).csv",
        "train_2016_Q1(2016 Q1).csv", "train_2016_Q2(2016 Q1).csv",
        "train_2016_Q3(2016 Q1).csv", "train_2016_Q4(2016 Q1).csv"};

#endif
#endif //GNQTS_STOCK_SLIDINGWINDOW_H
