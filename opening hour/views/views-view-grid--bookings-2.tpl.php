<?php

/**
 * @file
 * Default simple view template to display a rows in a grid.
 *
 * - $rows contains a nested array of rows. Each row contains an array of
 *   columns.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)) : ?>
<h3>
	<?php print $title; ?>
</h3>
<?php endif; ?>
<table class="<?php print $class; ?>" <?php print $attributes; ?>>
	<tbody>
		<tr>
		
			<?php 
			/* Begin Custom code to add headers for the grid columns
			 */
			if($view->args) {
       print"<th></th>";
       $new_args=explode(" ",$view->args[0]); //Create an array with number of hours
       $number_of_hours=count($new_args);
       $first_hour=reset($new_args);
       for ($i = 1; $i <= $number_of_hours; $i++) {
         print"<th>$first_hour</th>"; 
         $first_hour++;
       }
             }
      ?>
    <tr>   
			<?php foreach ($rows as $row_number => $columns): ?>
		
		
		<tr
		<?php if ($row_classes[$row_number]) { print 'class="' . $row_classes[$row_number] .'"';  } ?>>
		<td>ROOM NAMES HERE<?php  //have to figure out how to print out the room names here
			//dsm(strip_tags($view->result[0])); print_r("hello".$row_number); 
			?></td>
			<?php foreach ($columns as $column_number => $item): ?>
			<td
			<?php if ($column_classes[$row_number][$column_number]) { print 'class="' . $column_classes[$row_number][$column_number] .'"';  } ?>>
				<?php print $item; ?>
			</td>
			<?php endforeach; ?>
		</tr>
		<?php endforeach; ?>
	</tbody>
</table>

