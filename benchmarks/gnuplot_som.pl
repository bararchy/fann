#!/usr/bin/perl

$own_file = 0;
print '

set terminal postscript color solid
set output "quality_som.ps"';

print '
set style line 1 lt 1 lw 1
set style line 2 lt 2 lw 1
set style line 3 lt 3 lw 1
set style line 4 lt 4 lw 1
';


print '
set nologscale

set xlabel "Seconds of training"
set ylabel "Mean square error"
';

sub print_plot_line {
  local($filename, $title, $extra_title, $width, $last) = @_;

  print " \"$filename\" title \"$title $extra_title\" w l lt $linestyle lw $width";
  if(!$last)
  {
      print ", \\\n";
  }

  $linestyle++;
}


sub print_plot {
  local($train_file, $have_train, $own_file) = @_;

  if($own_file)
  {
      print "set output \"$train_file\_som.ps\"\n";
  }
  $width = 1;
  $extra_title="";
  $linestyle = 1;
  print "set title \"$train_file\"\n";
  print "plot ";
  print_plot_line("$train_file.hexagonal.distance.train.out", "Hexagonal Distance", $extra_title, $width, 0);
  print_plot_line("$train_file.hexagonal.gaussian.train.out", "Hexagonal Gaussian", $extra_title, $width, 0);
  print_plot_line("$train_file.rectangular.distance.train.out", "Rectangular Distance", $extra_title, $width, 0);
  print_plot_line("$train_file.rectangular.gaussian.train.out", "Rectangular Gaussian", $extra_title, $width, 1);
 print "\n";
 

  
}

for($own_file = 0; $own_file < 2; $own_file++)
{
    &print_plot("abelone", 1, $own_file);
    &print_plot("bank32fm", 1, $own_file);
    &print_plot("bank32nh", 1, $own_file);
    &print_plot("building", 1, $own_file);
    &print_plot("diabetes", 1, $own_file);
    &print_plot("kin32fm", 1, $own_file);
    &print_plot("pumadyn-32fm", 1, $own_file);
    &print_plot("robot", 1, $own_file);
    &print_plot("thyroid", 1, $own_file);
    &print_plot("two-spiral", 1, $own_file);
}


print "\nset output\n";
print "set terminal x11\n"
