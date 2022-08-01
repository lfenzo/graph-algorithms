using Test
using ArgParse


function parse_commandline()
    settings = ArgParseSettings()

    @add_arg_table! settings begin
        "--exercice", "-e"
            help = "Program to be compiled and executed"
            arg_type = String
            required = true
    end

    return parse_args(settings)
end


function main()
    args = parse_commandline()

    program_dir = "ex0" * args["exercice"]
    tmp_dir = "$program_dir/tmp"

    !isdir(tmp_dir) && mkdir(tmp_dir)
    
    # compile the C program 
    run(Cmd(`gcc main.c -o teste`, dir = program_dir))

    test_input_files = readdir(joinpath(program_dir, "in"))
    test_output_files = readdir(joinpath(program_dir, "out"))

    for (i, (test_input, test_output)) in enumerate(zip(test_input_files, test_output_files))

        input_file = joinpath(program_dir, "in", test_input)

        # exeuting the C program and retrieving its output in a temporary file
        run(pipeline(Cmd(`./teste`, dir = program_dir), stdin = input_file, stdout = "$tmp_dir/answer"))

        # check if the answer is the same as the expected output using linux's CMP command
        @info "Case $i >> $(success(`cmp --quiet $program_dir/out/$test_output $program_dir/tmp/answer`))"
    end
end


main()
