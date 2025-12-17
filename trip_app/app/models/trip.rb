class Trip < ApplicationRecord
  belongs_to :host, class_name: "User"
  validates :name, uniqueness: true
  validates :start_date, :end_date, presence: true
  validate :end_date_after_start_date

  has_many :user_trips, dependent: :destroy
  has_many :users, through: :user_trips
  has_many :expenses, dependent: :destroy

  private

  def end_date_after_start_date
    return if end_date.blank? || start_date.blank?

    if end_date < start_date
      errors.add(:end_date, "must be after start date")
    end
  end
end
