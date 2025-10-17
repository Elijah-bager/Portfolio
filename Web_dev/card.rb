module GameSet
  COLORS = %w[red green purple].freeze
  SHAPES = %w[diamond oval scribble].freeze
  SHADINGS = %w[solid striped open].freeze
  NUMBERS = [1, 2, 3].freeze
  ## Card class
  # Represents a single card in the Set game
  # Attributes: color, shape, shading, number, id
  # Each attribute must be from one of the predefined constants in the GameSet module
  class Card
    attr_accessor :color,   # color of the card, must be one of COLORS
                  :shape,   # shape of the card, must be one of SHAPES
                  :shading, # shading of the card, must be one of SHADINGS
                  :number   # number of shapes on the card, must be one of NUMBERS

    attr_reader :id # unique identifier for the card, see card_id! for details

    ## Card constructor
    # Constructs a new Card object
    # @param color [String] color of the card, must be one of COLORS
    # @param shape [String] shape of the card, must be one of SHAPES
    # @param shading [String] shading of the card, must be one of SHADINGS
    # @param number [Integer] number of shapes on the card, must be one of NUMBERS
    # @param id [Integer] unique identifier for the card, must be between 1 and 81
    # @return [Card] a new Card object
    def initialize(color, shape, shading, number)
      @color = color
      @shape = shape
      @shading = shading
      @number = number
      @id = card_id!
    end

    ## Card generator
    # Generates the full deck of 81 unique cards sorted by shape, color, shading, number
    # @structure Each card contains an unique ID (1-81) that associated with its combination of attributes.
    #   See card_id! for details.
    # @return [Array<Card>] array of 81 unique Card objects
    def self.card_generator
      master_cards = Array.new 0
      SHAPES.each do |s|
        COLORS.each do |c|
          SHADINGS.each do |h|
            NUMBERS.each do |n|
              card = Card.new(c, s, h, n)
              master_cards.push(card)
            end
          end
        end
      end
      master_cards #=> array of 81 unique cards, sorted by shape, color, shading, number
    end

    ## Card ID generator
    # Generates a unique ID for the card based on its attributes
    # The ID is calculated as follows:
    #   shape: diamond = 0, oval = 1, scribble = 2
    #   color: red = 0, green = 1, purple = 2
    #   shading: solid = 0, striped = 1, open = 2
    #   number: 1 = 0, 2 = 1, 3 = 2
    # The ID is then calculated as:
    #   id = shape * 27 + color * 9 + shading * 3 + number + 1
    # @return [Integer] unique ID for the card
    protected def card_id!
      shape_index = SHAPES.index(@shape)
      color_index = COLORS.index(@color)
      shading_index = SHADINGS.index(@shading)
      number_index = NUMBERS.index(@number)
      @id = (shape_index * 27) + (color_index * 9) + (shading_index * 3) + number_index + 1
    end

    ## Card to_s
    # Returns a string representation of the card based on detailed (1) or brief format (0, default)
    # @param detailed [Integer] 1 for detailed format, 0 for brief format (default)
    # @return [String] string representation of the card
    def to_s(detailed = 0)
      if detailed == 1
        "Card ID: #{@id}, Color: #{@color}, Shape: #{@shape}, Shading: #{@shading}, Number: #{@number}"
      else
        # unicode diamond: ♦, oval: ●, scribble: ~
        # terminal colors: red: \e[31m, green: \e[32m, purple: \e[35m, reset: \e[0m
        # shading being solid = +, striped = /, open = _
        # number being the number of shapes. i.e 3 solid diamonds being ♦+♦+♦+
        string_card = color_code
        string_shape = shape_code
        string_shading = shading_code
        @number.times { string_card.concat(string_shape + string_shading) }
        string_card.concat "\e[0m" # reset terminal color
        string_card # => brief string representation of the card
      end
    end

    ## Color code
    # Returns the terminal color format code for the card's color
    # @return [String] terminal color code for the card's color
    private def color_code
      color_string = ''
      case @color
      when 'red' then color_string = "\e[31m"
      when 'green' then color_string = "\e[32m"
      when 'purple' then color_string = "\e[35m"
      else puts 'Error: Invalid color'
      end
      color_string # => color format code
    end

    ## Shape code
    # Returns the unicode character for the card's shape
    # @return [String] unicode character for the card's shape
    private def shape_code
      shape_string = ''
      case @shape
      when 'diamond' then shape_string = '♦'
      when 'oval' then shape_string = '●'
      when 'scribble' then shape_string = '~'
      else puts 'Error: Invalid shape'
      end
      shape_string # => shape unicode character
    end

    ## Shading code
    # Returns the character for the card's shading
    # @return [String] character for the card's shading
    private def shading_code
      shading_string = ''
      case @shading
      when 'solid' then shading_string = '+'
      when 'striped' then shading_string = '/'
      when 'open' then shading_string = '_'
      else puts 'Error: Invalid shading'
      end
      shading_string # => shading character
    end
  end
end
