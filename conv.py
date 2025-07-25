import sys

def main():
    if len(sys.argv) < 2:
        print("Usage: python your_script_name.py <your_string>")
        sys.exit(1)

    input_string_utf8 = sys.argv[1]

    try:
        # Convert UTF-8 input string to GTK's preferred encoding (UTF-8 in most modern GTK versions)
        # Note: GTK itself primarily uses UTF-8 internally. If you were truly dealing with an older
        # or specific GTK context that used a different encoding (e.g., ISO-8859-1 for some legacy
        # widgets or platforms), you'd need to specify that here.
        # For modern GTK, a direct byte representation of the UTF-8 input is what's expected.
        gtk_bytes = input_string_utf8.encode('utf-8')

        # Format as a C string style hex representation
        c_string_hex = ""
        for byte in gtk_bytes:
            c_string_hex += f"\\x{byte:02x}"

        print(f'"{c_string_hex}"')

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

