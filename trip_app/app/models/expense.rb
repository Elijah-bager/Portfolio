class Expense < ApplicationRecord
  CATEGORIES = [
    "Food/Drink",
    "Lodging",
    "Gas",
    "Transportation",
    "Parking",
    "Activities",
    "Miscellaneous"
  ].freeze

  belongs_to :trip
  belongs_to :spender, class_name: "User"
  has_many :debts, dependent: :destroy

  validates :category, presence: true, inclusion: { in: CATEGORIES }
end
