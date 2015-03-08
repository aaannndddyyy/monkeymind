#!/bin/bash

# Generates packaging

rm -f Makefile rpmpackage/*.spec

packagemonkey -n "monkeymind" --cmd --dir "." -l "mit" -e "Bob Mottram (4096 bits) <bob@robotics.uk.to>" --brief "Mind of the monkey" --desc "Experiments with implementing a new cognitive system for multi-agent simulations" --homepage "https://github.com/bashrc/monkeymind" --repository "https://github.com/bashrc/monkeymind.git" --section "utils" --version "0.1" --categories "Utility/ConsoleOnly"
