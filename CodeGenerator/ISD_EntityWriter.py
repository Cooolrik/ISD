import CodeGeneratorHelpers

def ISD_EntityWriter_h():
    lines = []
    lines.append("""hello my baby hello my honey
    hello 
        my 
            ragtime 
                2""")
    CodeGeneratorHelpers.write_lines_to_file( "../test.cpp",lines)
