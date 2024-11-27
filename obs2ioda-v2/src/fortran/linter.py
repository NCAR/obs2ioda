import re
import sys

def reformat_fortran_function(code):
    """
    Reformats Fortran code so that each function argument is on a new line and
    the closing parenthesis is on a new line if the function has more than one argument.
    """
    def format_function(match):
        function_signature = match.group(1)
        arguments = match.group(2).split(',')
        closing_part = match.group(3)

        # Add newlines for each argument if there are more than one
        if len(arguments) > 1:
            formatted_args = ", &\n                ".join(arg.strip() for arg in arguments)
            return f"{function_signature}(\n                {formatted_args}\n            ){closing_part}"
        return match.group(0)  # Return unchanged if only one argument

    # Regular expression to find function declarations with arguments, including bind(C)
    function_pattern = re.compile(r"(\bfunction\b.*?\()([^\)]+)(\)\s*bind\(C.*)", re.DOTALL)

    # Apply formatting to all matches
    reformatted_code = function_pattern.sub(format_function, code)

    return reformatted_code

def main():
    if len(sys.argv) != 2:
        print("Usage: python reformat_fortran.py <fortran_file>")
        sys.exit(1)

    file_path = sys.argv[1]

    try:
        # Read the original Fortran code
        with open(file_path, 'r') as file:
            original_code = file.read()

        # Reformat the code
        reformatted_code = reformat_fortran_function(original_code)

        # Write the reformatted code back to the file
        with open(file_path, 'w') as file:
            file.write(reformatted_code)

        print(f"File '{file_path}' has been reformatted.")

    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

