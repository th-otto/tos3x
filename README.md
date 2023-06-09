# What is this?

These are the sources used to build orginal ROM versions of TOS 2.06/3.06,
the OS used by Atari STE and TT.

The goal of this project was to actually be able to
regenerate the original ROM images. 
The sources are not meant to be a base for serious further development, the Alcyon
compiler tools that were originally used are just too outdated and buggy.
They could be used to implement smaller patches/fixes.

It is therefor mainly intended to be used as reference.



# Directory Structure

- bin/tos:  
  Prebuild binaries of tools used in the build process. These are
  essentially identical to the ones from the developer kit, with 
  modifications as described in [alcyon/README](). Sources of almost
  all of these tools are found in the alcyon directory.

  It also contains a few tools that are not part of the developer-kit,
  but are now used in the build process (sed, rm, mv and bc). Those
  are taken from GNU packages.

- bin/linux:  
  Same tools as above, but compiled for linux-64bit. The cc68 executable
  in this directory is just a shell script that recognizes a few of
  the usual unix cc switches, and translates them to call the various
  alcyon executables.

- bin/win32:  
  Same tools as above, but compiled for windows-32bit.

- include:  
  Include files for the C compiler. Only a few of them are actually
  needed for TOS itself, most of them are only needed to build the
  tools.

- lib:  
  Prebuild libraries and startup files for the C runtime library.
  None of these is linked into a ROM image, they are only used for the tools.

- doc:  
  Some unsorted documentation.

- bios:  
  Source of the BIOS part of TOS.

- vdi:  
  Source of the VDI part of TOS.

- bdos:  
  Source of the GEMDOS part of TOS.

- aes:  
  Source of the AES part of TOS.

- desk:  
  Source of the desktop application.

- common:  
  Shared include files for the above components.

- tools:  
  Source of some small tools used in the build process,
  namely the one that glues the builtin resources and
  newdesk/desktop.inf files together. If you are cross-compiling,
  these must be compiled by your native host compiler.

- system:  
  Source of some small Atari tools.

- listings:  
  Disassembler listings of various TOS versions that were used while
  working on some parts of the system.

- glue:  
  The final ROM images will be build here. It also contains link maps
  for some ROM versions, for reference. The identifiers in those maps
  are truncated to 8 characters, though (a limitation of the DRI
  object file format).

- alcyon:  
  Source of the (modified) tools of the Alcyon C compiler & tools,
  as used to compile TOS.

- alcyon/orig:  
  Source of the tools of the Alcyon C compiler & tools, that compile to binary
  identical versions of the ones found in the developer kit.

(the contents of the alcyon directory are now available as a separate archive)


# Assembler sources

- Assembler sources are written in a format that is accepted
  both by as68 and by the GNU assembler with the --mri option.
  If you have to use a different assembler, they might need
  conversion.

- All comments in assembly files are in C-style. The makefile
  preprocesses them before passing them to the assembler. This
  avoids problems with different types of comment delimiters in
  different assemblers, and also works around some bugs of
  the conditional assembly directives of as68. It also allows
  using the same files defining some key constants to be
  used for both assembler and C.

- Don't put comments after constants that are used in assembler
  expressions. Since it is a K&R preprocessor, the spaces between
  the constant and the comment will make it to the source, and
  the assembler does not like spaces in operands.

- Assembling the assembler sources in the bios directory with
  the original as68 from the developer kit will not work, mostly
  because it does not support abs.w addressing mode, but also due
  to numerous bugs. Either use the fixed version from the bin
  directory or try some other assembler.


# Supported versions

BIOS, VDI, GEMDOS, AES, DESKTOP: 2.06, 2.08, 3.06
2.08 is 2.06 modified for ST-Book, it identifies itself as 2.06 though


# Supported language versions

- us - USA
- de - Germany
- fr - France
- uk - United Kingdom (uses US resources)
- es - Spain (no ROM yet found to verify)
- it - Italy (no ROM yet found to verify)
- se - Sweden
- fi - Finland (uses swedish keyboard and resources)
- no - Norway (uses swedish keyboard and resources)
- sf - Switzerland (French) (uses french keyboard and resources)
- sg - Switzerland (German) (uses german keyboard and resources)

- pl - Poland (only unofficial version 3.06 found on the net)
- cz - Czech (originally was a patched german version)


# How to compile

First off to clarify: the goal of this project was to actually be able to
regenerate the original ROM images. Most parts of the sources used herein were
already available before, but scattered around in several archives, and most
of the time incomplete so they could be used for illustratve purposes at best.
They are not meant to be a base for serious further development, the Alcyon
compiler tools that were originally used are just too outdated and buggy.

Having that said, if you still feeling adventurous and want to try to compile
it yourself:

- unpack the archive in some directory of your choice. All pathnames used
  in the makefiles are relative, so its name does not matter. Sources,
  object files and any built files will need ~13MB of disk space.

- install some shell. In my case i used GEMINI (unfortunately german only).
  bash might work, but some of the tools will have problems with MiNT which
  bash requires. Using command.tos from the bin directory should also work.

- cd to the toplevel directory

- make sure the bin directory from the archive is on your PATH:
  PATH=$PWD\bin,$PATH
  (for gemini/mupfel; other shells might use ';' as path separator)

- type "make"
  This should build the default configuration (german TOS 3.06).
  If the build succeeds, the final image will be found in the "glue" directory.

- for building other configurations, you can say e.g.
  make TOSVERSION=206 COUNTRY=us
  Alternatively, you can edit config.mak in the toplevel directory.
  See there also for a list of valid configurations.
  Also, when switching configurations, always make sure to run
  make clean
  first.
  If you pass the configuration via commandline, be sure to always
  use the same command when having to restart make.

- There might currently be some issues with \ vs / in pathnames. If
  that is the case, try to run it using some emulator. Hatari
  (https://hatari.tuxfamily.org/) is reported to work.

- There might be some more hints available on
  http://www.atari-forum.com/viewforum.php?f=16

- Be patient. On a STE, recompiling everything from scratch is reported
  to take ~2h, on a TT ~34 min. A Ramdisk might help, but note that the
  makefiles currently don't support to only put temporary files there.
  Without modifiying the makefiles you would need to put the sources there
  too, risking to loose any modifications you made if something goes awfully
  wrong.
