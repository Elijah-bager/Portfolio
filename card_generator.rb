shape_list = %w[diamond oval scribble]
color_list = %w[red green blue]
shade_list = %w[solid striped open]
number_list = [1, 2, 3]

master_cards = Array.new 0
i = 0
shape_list.each do |s|
  color_list.each do |c|
    shade_list.each do |h|
      number_list.each do |n|
        i += 1
        puts "card_#{i}"
        card = { shape: s, color: c, shade: h, num: n, id: i }
        master_cards.push(card)
      end
    end
  end
end

master_cards.each { |d| puts d }
