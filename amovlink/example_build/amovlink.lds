SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    __AMOVLinkCBTab_start = .;
    KEEP(*(.AMOVLinkCBTab))
    __AMOVLinkCBTab_end = .;
    . = ALIGN(4);
  }
}