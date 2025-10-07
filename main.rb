require_relative 'card'
require_relative 'player'
require_relative 'valid_set'
module Main
    include 
    class Game
        attr_reader :deck, :table, :player_1_score, :player_1_score, :player_2_score, :player_3_score,
        :player_4_score
        
        def initialize
            @deck=Card.card_generator.shuffle
            @table=[]
            @player_1_score=0
            @player_2_score=0
            @player_3_score=0
            @player_4_score=0
            create_table
        end

        def create_table
            12.times { @table << @deck.pop }
        end

        def show_table
            puts "\n Current deck"
            @table.each_with_index do |cards, i|
                puts "#{i+1}. #{Card.cards.to_s}"
            end

        def play
          show_table

          print "/n Pick a number 1-4 to choose player"
          player = gets.chomp.to_i
          if player.any? {|i| i<1 || i>4}
            puts "Incorrect. Pick a number 1-4"
            return
          end

          print "\n pick three cards by their ID"
          inputs=gets.chomp.split.map{&:to_i}
          if inputs !=3 || inputs.any? {|i| i<1 || i>@table.size}
          puts "Incorrect choices. Try again."
          return
          end
        choices = inputs.map {|i| @table[i-1]}
        if valid_set?(choices) 
          puts "CORRECT!"
          case player
          when 1
            @player_1_score+=1
          when 2
            @player_2_score+=1
          when 3
            @player_3_score+=1
          when 4
            @player_4_score+=1
          end
          re_deal
        
         else 
          puts "INCORRECT!"
        end
      
        def re_deal

        end


            
          end