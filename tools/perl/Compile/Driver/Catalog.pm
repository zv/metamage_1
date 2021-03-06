package Compile::Driver::Catalog;

use Compile::Driver::Files;
use Compile::Driver::InputFile::Catalog;
use Compile::Driver::InputFile::Description;

use warnings;
use strict;


*list_files         = \&Compile::Driver::Files::list;
*make_ancestor_dirs = \&Compile::Driver::Files::make_ancestor_dirs;

*read_catalog_file     = \&Compile::Driver::InputFile::Catalog::read_file;
*read_description_file = \&Compile::Driver::InputFile::Description::read_file;


my $subpath = "var/cache/compile-driver";

my $File = "$ENV{HOME}/$subpath/catalog";

my %Catalog;

my $new_catalog_data = "";

if ( -f $File )
{
	%Catalog = %{ read_catalog_file( $File ) };
}
else
{
	create_catalog_file();
}


sub add_project_description
{
	my ( $dir, $path ) = @_;
	
	my ( $name ) = $dir =~ m{ ( [^/]+ ) $}x;
	
	my $data = read_description_file( $path );
	
	if ( my $explicit_name = $data->{name}[0] )
	{
		$name = $explicit_name;
	}
	
	my $for = 0;
	my $not = 0;
	
	my $platform = "$for/$not";
	
	my $entry =
	{
		NAME => $name,
		FOR  => $for,
		NOT  => $not,
		PATH => $path,
	};
	
	push @{ $Catalog{ $name } }, $entry;
	
	$new_catalog_data .= join( "\t", $name, $platform, $path ) . "\n";
	
	my @subprojects = @{ $data->{subprojects} || [] };
	
	scan_for_project_descriptions( "$dir/$_" )  foreach @subprojects;
}

sub scan_for_project_descriptions
{
	my ( $dir ) = @_;
	
	my $conf = "$dir/A-line.conf";
	
	if ( -f $conf )
	{
		add_project_description( $dir, $conf );
		
		return;
	}
	
	my $confd = "${conf}d";
	
	if ( -d $confd )
	{
		my @confs = list_files( $confd, sub { / \. conf $/x } );
		
		add_project_description( $dir, $_ )  foreach @confs;
		
		return;
	}
	
	my @subdirs = list_files( $dir, sub { -d } );
	
	scan_for_project_descriptions( $_ )  foreach @subdirs;
}

sub create_catalog_file
{
	my $root = "$ENV{HOME}/src/tree";
	
	scan_for_project_descriptions( $root );
	
	my $path = $File;
	
	make_ancestor_dirs( $path );
	
	open my $out, ">", $path or die "$0: $path: $!\n";
	
	print $out $new_catalog_data or die "$0: $path: $!\n";
	
	close $out or die "$0: $path: $!\n";
}

sub find_project
{
	my ( $name ) = @_;
	
	my $entry = $Catalog{ $name };
	
	return  if !defined $entry;
	
	my @matches = @$entry;
	
	return 0  if !@matches;
	
	$entry = $matches[0];
	
	if ( !exists $entry->{DATA} )
	{
		$entry->{DATA} = read_description_file( $entry->{PATH} );
	}
	
	return $entry;
}

1;

