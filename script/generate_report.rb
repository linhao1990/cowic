#!/usr/bin/ruby

$log_file = "dataset/fp.log"
$report_file = "report/fp.csv"
$sep = ","
$bin = "bin/compressor_cmd_tool"

def generate_head()
    log_info = `wc #{$log_file}`
    head_line = "seed_entry_num#{$sep}expand_ratio#{$sep}trainTime(s)#{$sep}model_size(B)#{$sep}compress_time(s)#{$sep}data_size(B)#{$sep}data_ratio(%)#{$sep}total_ratio(%)#{$sep}decompress_time(s)"
    File.open($report_file, 'w') do |file|
        file.puts(log_info)
        file.puts(head_line)
    end
end

def file_entry_num(filename)
    /^ *\d+ /.match(`wc -l #{filename}`).to_s.to_i
end

def file_byte_num(filename)
    /^ *\d+ /.match(`wc -c #{filename}`).to_s.to_i
end

def measure_exe_time(cmd)
    startTime = Time.now
    puts cmd
    print `#{cmd}`
    endTime = Time.now
    return endTime - startTime
end

def generate_one_entry(expand_ratio)
    #genrate seed
    seed_ratio = 1.0 / expand_ratio 
    seed_entry_num = ($log_entry_num * seed_ratio).round
    seed_file_name = $log_file + ".#{seed_ratio}.seed"
    `head -#{seed_entry_num} #{$log_file} > #{seed_file_name}` unless File.exists? seed_file_name

    #train 
    model_file_name = $log_file + ".#{seed_ratio}.mdl"
    `rm #{model_file_name}*`
    train_cmd = "#{$bin} -t #{seed_file_name} -m #{model_file_name}"
    train_time = measure_exe_time train_cmd

    #compress
    output_file_name = $log_file + ".#{seed_ratio}.out"
    compress_cmd = "#{$bin} -c #{$log_file} -m #{model_file_name} -o #{output_file_name}"
    compress_time = measure_exe_time compress_cmd

    model_byte_num = file_byte_num model_file_name
    data_byte_num = file_byte_num(output_file_name + ".dat")
    data_ratio = ((data_byte_num + 0.0) / $log_byte_num) * 100
    total_ratio = ((data_byte_num + model_byte_num + 0.0) / $log_byte_num) * 100

    #decompress
    raw_file_name = $log_file + ".#{seed_ratio}.raw"
    decompress_cmd = "#{$bin} -d #{output_file_name} -m #{model_file_name} -o #{raw_file_name}"
    decompress_time = measure_exe_time decompress_cmd

    diff = `diff #{$log_file} #{raw_file_name}`
    abort("#{$log_file} does not match decompress result #{raw_file_name}!") if diff.size > 0

    File.open($report_file, 'a') do |file|
        file.puts "#{seed_entry_num}#{$sep}#{expand_ratio}#{$sep}#{train_time}#{$sep}#{model_byte_num}#{$sep}#{compress_time}#{$sep}#{data_byte_num}#{$sep}#{data_ratio}#{$sep}#{total_ratio}#{$sep}#{decompress_time}"
    end
    puts "Succ in generate report for seed ratio: #{seed_ratio}!"
end

def generate_huffman_report()
    $log_entry_num = file_entry_num $log_file
    $log_byte_num = file_byte_num $log_file
    [1, 2, 5, 10, 20, 50, 100, 200, 500, 1000].reverse_each do |expand_ratio|
        generate_one_entry(expand_ratio)
    end
end

def genrate_report()
    generate_head()
    generate_huffman_report()
    puts "OK. Write result of #{$log_file} to #{$report_file}"
    puts `cat #{$report_file}`
end

if(ARGV.size == 2)
    $log_file = ARGV[0]
    $report_file = ARGV[1]
end

genrate_report()
