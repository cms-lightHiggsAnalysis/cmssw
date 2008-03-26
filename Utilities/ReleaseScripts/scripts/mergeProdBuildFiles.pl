#!/usr/bin/env perl
use File::Basename;
use lib dirname($0);
use Getopt::Long;
use SCRAMGenUtils;

$|=1;
if(&GetOptions(
	       "--dir=s",\$dir,
	       "--common",\$common,
	       "--help",\$help,
	      ) eq ""){print STDERR "#Wrong arguments.\n"; &usage_msg();}
if(defined $help){&usage_msg();}
if((!defined $dir) || ($dir=~/^\s*$/) || (!-d $dir)){print STDERR "Missing directory path where the newly auto generated BuildFiles exist.\n"; &usage_msg();}
if(defined $common){$common=1;}
else{$common=0;}
$dir=&SCRAMGenUtils::fixPath($dir);
my $release=&SCRAMGenUtils::scramReleaseTop($dir);
if(!-d "${release}/.SCRAM"){print STDERR "ERROR: $dir is not under a SCRAM-based project.\n"; exit 1;}
&SCRAMGenUtils::init ($release);
&process($dir);
exit 0;

sub process ()
{
  my $dir=shift || return;
  my $xml=shift || 0;
  if(!-d $dir){return;}
  my %bfs=();
  foreach my $file (&SCRAMGenUtils::readDir("$dir",0))
  {
    my $fpath="${dir}/${file}";
    if(-d $fpath){&process($fpath,$xml);}
    elsif($file=~/^(.+?)BuildFile(\.xml|)\.auto$/)
    {
      if($2 eq ".xml"){$xml=1;}
      $bfs{$file}=0;
    }
  }
  if(scalar(keys %bfs)==0){return;}
  print "Working on $dir\n";
  my %commontools=();
  if($common)
  {
    my $flag=0;
    foreach my $file (keys %bfs)
    {
      if(($flag) && (scalar(keys %{$commontools{use}})==0) && (scalar(keys %{$commontools{lib}})==0)){last;}
      my $bf=&SCRAMGenUtils::readBuildFile("${dir}/${file}");
      foreach my $type ("bin", "library")
      {
        if(exists $bf->{$type})
        {
          foreach my $prod (keys %{$bf->{$type}})
	  {
	    my %local=();
	    foreach my $x ("use","lib")
	    {
	      if(exists $bf->{$type}{$prod}{deps}{$x})
	      {foreach my $u (keys %{$bf->{$type}{$prod}{deps}{$x}}){$local{$x}{$u}=1;}}
	    }
	    if($flag==0)
	    {
	      foreach my $x ("use","lib")
	      {
	        foreach my $u (keys %{$local{$x}}){$commontools{$x}{$u}=1;}
	      }
	      $flag=1;
	    }
	    else
	    {
	      foreach my $x ("use","lib")
	      {foreach my $u (keys %{$commontools{$x}}){if(!exists $local{$x}{$u}){delete $commontools{$x}{$u};}}}
	    }
	  }
        }
      }
    }
  }
  my $mbf="${dir}/BuildFile.auto";
  if ($xml){$mbf="${dir}/BuildFile.xml.auto";}
  my $outfile;
  open($outfile,">$mbf") || die "Can not open \"$mbf\" for writing.";
  if($common)
  {
    foreach my $x ("use","lib")
    {
      foreach my $u (keys %{$commontools{$x}})
      {
        print $outfile "<$x name=\"$u\"";
        if ($xml){print $outfile "/";}
        print $outfile ">\n";
      }
    }
  }
  my $c=scalar(keys %bfs);
  print "Files:$c\n";
  foreach my $file (keys %bfs)
  {
    my $infile;
    open($infile,"${dir}/${file}") || die "Can not open \"${dir}/${file}\" for reading.";
    my $line="";
    while($line=$line || <$infile>)
    {
      chomp $line;
      if($common)
      {
        foreach my $x ("use","lib")
	{
	  foreach my $u (keys %{$commontools{$x}})
          {if($line=~/^\s*<$x\s+name=\"$u\"(\/|)\s*>(.*)$/){$line=$2;}}
	}
      }
      if($line!~/^\s*$/){print $outfile "$line\n";$line="";}
    }
    close($infile);
  }
  close($outfile);
}
	      
sub usage_msg() 
{
  my $script=basename($0);
  print "Usage: $script --dir <dir> [--common]\n\n";
  print "  --dir    <dir>  Path of the directory where the newly generated BuildFile.auto files exist.\n";
  print "  --common        To Search for common tools used by different products (library/bin) in a\n";
  print "                  test/bin area and add those tools once in the BuildFile.\n";
  exit 0;
}
