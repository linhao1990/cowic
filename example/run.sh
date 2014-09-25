#!/bin/bash

function echo_exec(){
    echo $1
    echo `$1`
}

echo_exec "./bin/compressor_cmd_tool -t dataset/fp.log -m dataset/mdl"
echo_exec "./bin/compressor_cmd_tool -c dataset/fp.log -m dataset/mdl -o dataset/output"
echo_exec "./bin/compressor_cmd_tool -d dataset/output -m dataset/mdl -o dataset/raw"
echo_exec "diff dataset/fp.log dataset/raw"
