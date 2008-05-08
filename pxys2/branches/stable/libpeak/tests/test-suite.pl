#!/usr/bin/perl -w
#
# PEAK Library - Test suite script
#
# Copyright (c) 2004
#      Jean-Edouard BABIN <Jeb@jeb.com.fr>. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# $Id: test-suite.pl,v 1.12 2004/01/17 22:26:19 radius Exp $

use strict;

select STDOUT;
$| = 1;

my %status;
use vars qw($test $alrm);
my $default_alarm;
require 'test.desc';

$SIG{'ALRM'} = \&sigALRM_handler;
$ENV{'PATH'} = '.';

$default_alarm = 1;
$status{error}=0;
$status{'file not found'}=0;
$status{"can't exec file"}=0;
$status{alarm}=0;
$status{warning}=0;
$status{successful}=0;
$status{'test is not defined'}=0;
$status{'unknow return code'}=0;
$status{'unknow timeout message'}=0;

print "\nTesting libpeak:\n";

for (@ARGV) {
	print "$_...\t";
	if (!$test->{$_}) {
		print "Error: This test is not defined\n";
		$status{'test is not defined'}++;
		$status{error}++;
		next;
	}
	if (!(-e $test->{$_}{file})) {
		print "Error: File not found\n";
		$status{'file not found'}++;
		$status{error}++;
		next;
	}
	if (!(-x $test->{$_}{file})) {
		print "Error: File can't be executed\n";
		$status{"can't exec file"}++;
		$status{error}++;
		next;
	}
	if ($test->{$_}{timeout}{time}) {
		alarm $test->{$_}{timeout}{time};
	}
	else {
		alarm $default_alarm;
	}
	$alrm = 0;
	system("$test->{$_}{file} 1>>STDOUT.log 2>>STDERR.log");
	alarm 0;
	if ($alrm) {
		if ($test->{$_}{timeout}{message}) {
			print "$test->{$_}{timeout}{message}\n";
		}
		else {
			print "Timeout: Unknow timeout message\n";
			$status{'unknow timeout message'}++;
		}
		foreach my $status_type (@{$test->{$_}{timeout}{status}}) {
			$status{$status_type}++;
		}
		next;
	}
	if (($?>>8) > 127) {
		# Anormal Quit, program got a signal
		print "Anormal Quit, program got signal " . (($?>>8)&127) . "\n";
		$status{'signal number ' . (($?>>8)&127)}++;
		$status{error}++;
		next;
	}
	if ($test->{$_}{code}{($?>>8)}{message}) {
		print "$test->{$_}{code}{($?>>8)}{message}\n";
	}
	else {
		print "Unknow code: " . ($?>>8) . "\n";
		$status{'unknow return code'}++;
	}
	foreach my $status_type (@{$test->{$_}{code}{($?>>8)}{status}}) {
		$status{$status_type}++;
	}
}

print "\nSummary:\n";
foreach (sort keys %status) {
	print "\t$status{$_} $_\n" if $status{$_};
}

sub sigALRM_handler {
        $alrm = 1;
}
