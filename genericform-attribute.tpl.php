<?php
/**
 * @file
 *
 * Theme for show a attribute
 */
?>
<p class="item-attribute">
  <strong><?php if(isset($name)): echo ucfirst($name); endif;?></strong>
  <span><?php if(isset($value)): echo ucfirst($value); endif;?></span>
</p>
