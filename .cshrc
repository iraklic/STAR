#
# General Purpose STAR cshrc file.
# Also valid for tcsh

#aklog


# The default STAR_LEVEL and CERN_LEVEL are named 'pro' i.e.
# stable production mode versions.
# You can overwrite the defaults by setting the following 
# environment variables (un-comment the lines below) ...

if ($?STAR_LEVEL == 0) setenv STAR_LEVEL DEV
# if ($?CERN_LEVEL == 0) setenv CERN_LEVEL new

# By default, the STAR login displays information the first 
# time you log-in. However, you may over-write this (and have
# no messages at all) by uncommenting the next line

# setenv  SILENT 1
if ($?GROUP_DIR == 0) setenv GROUP_DIR /afs/rhic/rhstar/group
#setenv GARFIELD_HOME /gpfs01/star/pwg/iraklic/GARFIELD

setenv GARFIELD_HOME /star/u/iraklic/STAR/Garfield

if ( -r  $GROUP_DIR/star_cshrc.csh ) then
       	source $GROUP_DIR/star_cshrc.csh
endif


# How can you change your prompt:
#================================
# Definition		 	# Result	# Comment
#-------------------------------------------------------------------
# set prompt="$HOST "	 	# plus1         # 
# set prompt='\! > '	 	# 3 >		# 3 means the third 
#			 	# 		# you executed
# set prompt='\! %~ %# '	# 3 ~/public >	# 
# set prompt="[$HOST] > "	# [plus1] > 	#
# set prompt='R; '		# R;		# for VM hackers!
#

# Default aliases
# ===============
# Some default aliases are defined. You may remove
# them by commenting this out.
alias cls   'clear'             # clear screen
alias ls    'ls -F'             # show '@' after a linked file 
                                # '/' after a dir


# Changing the shell behaviour:
#==============================
# Normally, redirecting command output to a file, for example
#
#      /bin/ls > file.list
#
# will OVERWRITE any existing file.list. As the default you can't overwrite
# existing files. If you dislike it uncomment the following line:
#
# unset noclobber


# By default you need to type "logout" to logout from a standard Unix
# system. If you prefer to logout with a simple "CTRL-D" you should
#
# unset ignoreeof

#my commands
alias fsize 'du -hs'
alias myweb 'cd /afs/rhic.bnl.gov/star/users/iraklic/WWW'
alias myJobs 'condor_q -global iraklic | grep iraklic -B 2 -A 2'
alias myProtectedArea 'cd /afs/rhic.bnl.gov/star/doc_protected/www/'
alias myScratch 'cd /gpfs01/star/scratch/iraklic'
alias myPWG 'cd /gpfs01/star/pwg/iraklic'
alias myTPCArea 'cd /star/subsys/tpc/iraklic'
alias myBNLArea 'cd /star/institutions/bnl/iraklic'
alias files2web 'perl /star/u/iraklic/private/files2web.pl'
alias mix2web 'perl /star/u/iraklic/private/mix2web.pl'
alias ed2web 'perl /star/u/iraklic/private/ed2web.pl'
alias pics2web 'perl /star/u/iraklic/private/pics2web.pl'
alias myDEV2 'starver .DEV2; source $STAR/setupDEV2.csh; starver .DEV2'
alias uniqueList 'perl /star/u/iraklic/STAR/macros/HPSS/uniqueList.pl'
alias hpssCopyList 'perl /star/u/iraklic/STAR/macros/HPSS//hppsCopyList.pl'
alias alignIt 'root -q -b ~/STAR/macros/TpcAlignerDraw.C+'
alias getAlignmentResults 'root -q -b ~/STAR/macros/getAlignmentParams.C'
alias getSSAlignmetnResults 'source ~/STAR/macros/SSAlign.sh'
alias getSSParams 'source ~/STAR/macros/getSSParams.sh'
alias makeCosmicT 'root -l -q -b lMuDst.C ~/STAR/macros/Cosmics.C+'
alias diffy 'diff -y -W $(( $(tput cols) - 2 ))'

#FOR PROMPT HITS

setenv MYPWG /gpfs01/star/pwg/iraklic
