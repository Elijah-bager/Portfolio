class Debt < ApplicationRecord
 belongs_to :expense
  belongs_to :debtor, class_name: "User"
end
