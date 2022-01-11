echo "// Generated file - do not edit."
echo "#include \"idt.h\""
echo "#include \"terminal.h\""

# Generate ISRs.
for interrupt in `seq 0 255`;
do
    echo "__attribute__((weak))"
    echo "void isr_${interrupt}_handler(unsigned interrupt, void* frame)"
    echo "{"
    echo "    terminal_write_string(\"Unhandled interrupt: $interrupt\\n\");"
    echo "}"

    echo "__attribute__((interrupt))"
    echo "static void isr_${interrupt}(void* frame)"
    echo "{"
    echo "    isr_${interrupt}_handler($interrupt, frame);"
    echo "}"
done

echo "void isr_install_handlers(void)"
echo "{"
for interrupt in `seq 0 255`;
do
    echo "    idt_set_entry($interrupt, &isr_$interrupt);"
done
echo "}"
